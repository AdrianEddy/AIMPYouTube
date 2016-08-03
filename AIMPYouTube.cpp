#include "AIMPYouTube.h"

#include "AIMPString.h"
#include "SDK/apiPlayer.h"
#include "Timer.h"
#include "PlaylistListener.h"
#include "AddURLDialog.h"
#include "Tools.h"
#include "AimpHTTP.h"
#include "YouTubeAPI.h"
#include "AimpMenu.h"
#include "resource.h"
#include "PlayerHook.h"
#include "FileSystem.h"
#include "ArtworkProvider.h"
#include <set>
#include <ctime>

HRESULT __declspec(dllexport) WINAPI AIMPPluginGetHeader(IAIMPPlugin **Header) {
    *Header = Plugin::instance();
    return S_OK;
}

Plugin *Plugin::m_instance = nullptr;

HRESULT WINAPI Plugin::Initialize(IAIMPCore *Core) {
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    if (Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL) != Gdiplus::Status::Ok)
        return E_FAIL;

    m_core = Core;
    AIMPString::Init(Core);

    if (FAILED(m_core->QueryInterface(IID_IAIMPServiceMUI, reinterpret_cast<void **>(&m_muiService)))) {
        Finalize();
        return E_FAIL;
    }

      if (!Config::Init(Core)) { Finalize(); return E_FAIL; }
    if (!AimpHTTP::Init(Core)) { Finalize(); return E_FAIL; }
    if (!AimpMenu::Init(Core)) { Finalize(); return E_FAIL; }

    Config::LoadExtendedConfig();

    m_accessToken = Config::GetString(L"AccessToken");
    m_refreshToken = Config::GetString(L"RefreshToken");
    m_tokenExpireTime = Config::GetInt64(L"TokenExpires");

    if (!m_refreshToken.empty() && m_tokenExpireTime - 30 <= std::time(nullptr)) {
        refreshAccessToken();
    }

    if (AimpMenu *addMenu = AimpMenu::Get(AIMP_MENUID_PLAYER_PLAYLIST_ADDING)) {
        addMenu->Add(Lang(L"YouTube.Menu\\AddURL"), [this](IAIMPMenuItem *) { AddURLDialog::Show(); }, IDB_ICON)->Release();
        delete addMenu;
    }

    auto enableIfValid = [this](IAIMPMenuItem *item) {
        int valid = 0;
        ForSelectedTracks([&valid](IAIMPPlaylist *, IAIMPPlaylistItem *, const std::wstring &id) -> int {
            if (!id.empty()) valid++;
            return 0;
        });

        item->SetValueAsInt32(AIMP_MENUITEM_PROPID_VISIBLE, valid > 0);
    };

    if (AimpMenu *contextMenu = AimpMenu::Get(AIMP_MENUID_PLAYER_PLAYLIST_CONTEXT_FUNCTIONS)) {
        auto fs = new FileSystem(m_core);
        if (FAILED(m_core->RegisterExtension(IID_IAIMPServiceFileSystems, static_cast<FileSystem::Base *>(fs)))) {
            delete fs;

            contextMenu->Add(Lang(L"YouTube.Menu\\OpenInBrowser"), [this](IAIMPMenuItem *) {
                ForSelectedTracks([this](IAIMPPlaylist *, IAIMPPlaylistItem *, const std::wstring &id) -> int {
                    if (auto ti = Tools::TrackInfo(id)) {
                        if (!ti->Permalink.empty())
                            ShellExecute(GetMainWindowHandle(), L"open", ti->Permalink.c_str(), NULL, NULL, SW_SHOWNORMAL);
                    }
                    return 0;
                });
            }, IDB_ICON, enableIfValid)->Release();
        }

        contextMenu->Add(Lang(L"YouTube.Menu\\AddToExclusions"), [this](IAIMPMenuItem *) {
            ForSelectedTracks([](IAIMPPlaylist *, IAIMPPlaylistItem *, const std::wstring &id) -> int {
                if (!id.empty()) {
                    Config::TrackExclusions.insert(id);
                    return FLAG_DELETE_ITEM;
                }
                return 0;
            });
            Config::SaveExtendedConfig();
        }, IDB_ICON, enableIfValid)->Release();

        delete contextMenu;
    }

    if (AimpMenu *miscMenu = AimpMenu::Get(AIMP_MENUID_PLAYER_PLAYLIST_MISCELLANEOUS)) {
        miscMenu->Add(Lang(L"YouTube.Menu\\CheckForNewItems"), [this](IAIMPMenuItem *) {
            MonitorCallback();
        }, IDB_ICON)->Release();

        delete miscMenu;
    }

    if (FAILED(m_core->QueryInterface(IID_IAIMPServicePlaylistManager, reinterpret_cast<void **>(&m_playlistManager)))) {
        Finalize();
        return E_FAIL;
    }

    if (FAILED(m_core->QueryInterface(IID_IAIMPServiceMessageDispatcher, reinterpret_cast<void **>(&m_messageDispatcher)))) {
        Finalize();
        return E_FAIL;
    }

    m_messageHook = new MessageHook(this);
    if (FAILED(m_messageDispatcher->Hook(m_messageHook))) {
        delete m_messageHook;
        Finalize();
        return E_FAIL;
    }

    if (FAILED(m_core->RegisterExtension(IID_IAIMPServiceOptionsDialog, static_cast<OptionsDialog::Base *>(new OptionsDialog(this))))) {
        Finalize();
        return E_FAIL;
    }
 
    if (FAILED(m_core->RegisterExtension(IID_IAIMPServicePlaylistManager, new PlaylistListener()))) {
        Finalize();
        return E_FAIL;
    }

    if (FAILED(m_core->RegisterExtension(IID_IAIMPServicePlayer, new PlayerHook()))) {
        Finalize();
        return E_FAIL;
    }
    if (FAILED(m_core->RegisterExtension(IID_IAIMPServiceAlbumArt, new ArtworkProvider()))) {
        Finalize();
        return E_FAIL;
    }

    if (Config::GetInt32(L"CheckOnStartup", 1)) {
        Timer::SingleShot(2000, MonitorCallback);
    }

    StartMonitorTimer();
    UpdatePlaylistMenu();

    return S_OK;
}

void Plugin::StartMonitorTimer() {
    bool enabled = Config::GetInt32(L"CheckEveryEnabled", 1) == 1;
    if (m_monitorTimer > 0 && enabled)
        return;

    KillMonitorTimer();
    if (enabled) {
        m_monitorTimer = Timer::Schedule(Config::GetInt32(L"CheckEveryHours", 1) * 60 * 60 * 1000, MonitorCallback);
    }
}

void Plugin::KillMonitorTimer() {
    if (m_monitorTimer > 0) {
        Timer::Cancel(m_monitorTimer);
        m_monitorTimer = 0;
    }
}

void Plugin::MonitorCallback() {
    if (m_instance->m_monitorPendingUrls.empty()) {
        for (const auto &x : Config::MonitorUrls) {
            m_instance->m_monitorPendingUrls.push(x);
        }
        if (m_instance->isConnected()) {
            std::wstring auth = L"\r\nAuthorization: Bearer " + m_instance->m_accessToken;

            // Load user playlists
            AimpHTTP::Get(L"https://www.googleapis.com/youtube/v3/playlists?part=snippet&maxResults=50&mine=true&fields=items(id%2Csnippet)" + auth, [](unsigned char *data, int size) {
                rapidjson::Document d;
                d.Parse(reinterpret_cast<const char *>(data));

                if (d.IsObject() && d.HasMember("items") && d["items"].IsArray() && d["items"].Size() > 0) {
                    for (auto x = d["items"].Begin(), e = d["items"].End(); x != e; x++) {
                        const rapidjson::Value &item = *x;
                        std::wstring id(Tools::ToWString(item["id"]));
                        auto find = [&](const Config::Playlist &p) -> bool { return p.ID == id; };
                        if (std::find_if(Config::UserPlaylists.begin(), Config::UserPlaylists.end(), find) == Config::UserPlaylists.end()) {
                            Config::UserPlaylists.push_back({ id, Tools::ToWString(item["snippet"]["localized"]["title"]), true });
                        }
                    }
                }
                m_instance->UpdatePlaylistMenu();
            });
        }
    }
    if (!m_instance->m_monitorPendingUrls.empty()) {
        const Config::MonitorUrl &url = m_instance->m_monitorPendingUrls.front();
        IAIMPPlaylist *pl = m_instance->GetPlaylistById(url.PlaylistID, false);
        if (!pl) {
            m_instance->m_monitorPendingUrls.pop();
            if (!m_instance->m_monitorPendingUrls.empty())
                MonitorCallback();
            return;
        }

        auto state = std::make_shared<YouTubeAPI::LoadingState>();
        state->ReferenceName = url.GroupName;
        state->Flags = url.Flags;

        YouTubeAPI::GetExistingTrackIds(pl, state);
        if (m_instance->m_monitorPendingUrls.size() > 1) {
            YouTubeAPI::LoadFromUrl(url.URL, pl, state, MonitorCallback);
        } else {
            YouTubeAPI::LoadFromUrl(url.URL, pl, state); // last one without callback
        }
        m_instance->m_monitorPendingUrls.pop();
    }
}

HRESULT WINAPI Plugin::Finalize() {
    Timer::StopAll();

    AimpMenu::Deinit();
    AimpHTTP::Deinit();
    Config::Deinit();

    if (m_messageDispatcher) {
        m_messageDispatcher->Unhook(m_messageHook);
        m_messageDispatcher->Release();
        m_messageDispatcher = nullptr;
    }
    if (m_muiService) {
        m_muiService->Release();
        m_muiService = nullptr;
    }

    if (m_playlistManager) {
        m_playlistManager->Release();
        m_playlistManager = nullptr;
    }

    Gdiplus::GdiplusShutdown(m_gdiplusToken);
    m_finalized = true;
    return S_OK;
}

IAIMPPlaylist *Plugin::GetPlaylistById(const std::wstring &playlistId, bool activate) {
    IAIMPPlaylist *playlistPointer = nullptr;
    if (SUCCEEDED(m_playlistManager->GetLoadedPlaylistByID(AIMPString(playlistId), &playlistPointer)) && playlistPointer) {
        if (activate)
            m_playlistManager->SetActivePlaylist(playlistPointer);

        return UpdatePlaylistGrouping(playlistPointer);
    }

    return nullptr;
}


IAIMPPlaylist *Plugin::GetPlaylist(const std::wstring &playlistName, bool activate, bool create) {
    IAIMPPlaylist *playlistPointer = nullptr;
    if (SUCCEEDED(m_playlistManager->GetLoadedPlaylistByName(AIMPString(playlistName), &playlistPointer)) && playlistPointer) {
        if (activate)
            m_playlistManager->SetActivePlaylist(playlistPointer);

        return UpdatePlaylistGrouping(playlistPointer);
    } else if (create) {
        if (SUCCEEDED(m_playlistManager->CreatePlaylist(AIMPString(playlistName), activate, &playlistPointer)))
            return UpdatePlaylistGrouping(playlistPointer);
    }

    return nullptr;
}

IAIMPPlaylist *Plugin::GetCurrentPlaylist() {
    IAIMPPlaylist *pl = nullptr;
    if (SUCCEEDED(m_playlistManager->GetActivePlaylist(&pl))) {
        return pl;
    }
    return nullptr;
}

void Plugin::ForAllPlaylists(std::function<void(IAIMPPlaylist *, const std::wstring &)> cb) {
    if (!cb)
        return;

    int count = m_playlistManager->GetLoadedPlaylistCount();
    for (int i = 0; i < count; ++i) {
        IAIMPPlaylist *pl = nullptr;
        if (SUCCEEDED(m_playlistManager->GetLoadedPlaylist(i, &pl))) {
            std::wstring name;
            IAIMPPropertyList *plProp = nullptr;
            if (SUCCEEDED(pl->QueryInterface(IID_IAIMPPropertyList, reinterpret_cast<void **>(&plProp)))) {
                IAIMPString *id = nullptr;
                if (SUCCEEDED(plProp->GetValueAsObject(AIMP_PLAYLIST_PROPID_NAME, IID_IAIMPString, reinterpret_cast<void **>(&id)))) {
                    name = id->GetData();
                    id->Release();
                }
                plProp->Release();
            }

            cb(pl, name);
        }
    }
}

IAIMPPlaylist *Plugin::UpdatePlaylistGrouping(IAIMPPlaylist *pl) {
    if (!pl)
        return nullptr;

    // Change playlist grouping template to %A (album)
    IAIMPPropertyList *plProp = nullptr;
    if (SUCCEEDED(pl->QueryInterface(IID_IAIMPPropertyList, reinterpret_cast<void **>(&plProp)))) {
        plProp->SetValueAsInt32(AIMP_PLAYLIST_PROPID_GROUPPING_OVERRIDEN, 1);
        plProp->SetValueAsObject(AIMP_PLAYLIST_PROPID_GROUPPING_TEMPLATE, AIMPString(L"%A"));
        plProp->Release();
    }
    return pl;
}

std::wstring Plugin::PlaylistId(IAIMPPlaylist *pl) {
    std::wstring playlistId;
    IAIMPPropertyList *plProp = nullptr;
    if (SUCCEEDED(pl->QueryInterface(IID_IAIMPPropertyList, reinterpret_cast<void **>(&plProp)))) {
        IAIMPString *id = nullptr;
        if (SUCCEEDED(plProp->GetValueAsObject(AIMP_PLAYLIST_PROPID_ID, IID_IAIMPString, reinterpret_cast<void **>(&id)))) {
            playlistId = id->GetData();
            id->Release();
        }
        plProp->Release();
    }
    return playlistId;
}

IAIMPPlaylistItem *Plugin::GetCurrentTrack() {
    IAIMPServicePlayer *player = nullptr;
    if (SUCCEEDED(m_core->QueryInterface(IID_IAIMPServicePlayer, reinterpret_cast<void **>(&player)))) {
        IAIMPPlaylistItem *item = nullptr;
        if (SUCCEEDED(player->GetPlaylistItem(&item))) {
            player->Release();
            return item;
        }
        player->Release();
    }
    return nullptr;
}

void Plugin::ForSelectedTracks(std::function<int(IAIMPPlaylist *, IAIMPPlaylistItem *, const std::wstring &)> callback) {
    if (!callback)
        return;

    if (IAIMPPlaylist *pl = GetCurrentPlaylist()) {
        pl->BeginUpdate();
        std::set<IAIMPPlaylistItem *> to_del;
        auto delPending = [&] {
            for (auto x : to_del) {
                pl->Delete(x);
                x->Release();
            }
        };
        for (int i = 0, n = pl->GetItemCount(); i < n; ++i) {
            IAIMPPlaylistItem *item = nullptr;
            if (SUCCEEDED(pl->GetItem(i, IID_IAIMPPlaylistItem, reinterpret_cast<void **>(&item)))) {
                int isSelected = 0;
                if (SUCCEEDED(item->GetValueAsInt32(AIMP_PLAYLISTITEM_PROPID_SELECTED, &isSelected))) {
                    if (isSelected) {
                        IAIMPString *url = nullptr;
                        if (SUCCEEDED(item->GetValueAsObject(AIMP_PLAYLISTITEM_PROPID_FILENAME, IID_IAIMPString, reinterpret_cast<void **>(&url)))) {
                            std::wstring id = Tools::TrackIdFromUrl(url->GetData());
                            url->Release();
                            
                            int result = callback(pl, item, id);
                            if (result & FLAG_DELETE_ITEM) {
                                to_del.insert(item);
                                if (result & FLAG_STOP_LOOP) {
                                    delPending();
                                    pl->EndUpdate();
                                    pl->Release();
                                    return;
                                }
                                continue;
                            }
                            if (result & FLAG_STOP_LOOP) {
                                delPending();
                                item->Release();
                                pl->EndUpdate();
                                pl->Release();
                                return;
                            }
                        }
                    }
                }
                item->Release();
            }
        }
        delPending();
        pl->EndUpdate();
        pl->Release();
    }
}

void Plugin::ForEveryItem(IAIMPPlaylist *pl, std::function<int(IAIMPPlaylistItem *, IAIMPFileInfo *, const std::wstring &)> callback) {
    if (!pl || !callback)
        return;

    pl->BeginUpdate();
    for (int i = 0, n = pl->GetItemCount(); i < n; ++i) {
        IAIMPPlaylistItem *item = nullptr;
        if (SUCCEEDED(pl->GetItem(i, IID_IAIMPPlaylistItem, reinterpret_cast<void **>(&item)))) {
            IAIMPFileInfo *finfo = nullptr;
            if (SUCCEEDED(item->GetValueAsObject(AIMP_PLAYLISTITEM_PROPID_FILEINFO, IID_IAIMPFileInfo, reinterpret_cast<void **>(&finfo)))) {
                IAIMPString *custom = nullptr;
                if (SUCCEEDED(finfo->GetValueAsObject(AIMP_FILEINFO_PROPID_FILENAME, IID_IAIMPString, reinterpret_cast<void **>(&custom)))) {
                    std::wstring url(custom->GetData());
                    custom->Release();

                    std::wstring id = Tools::TrackIdFromUrl(url);
                    int result = callback(item, finfo, id);
                    if (result & FLAG_DELETE_ITEM) {
                        pl->Delete(item);
                        finfo->Release();
                        item->Release();
                        i--;
                        n--;
                        if (result & FLAG_STOP_LOOP) {
                            pl->EndUpdate();
                            return;
                        }
                        continue;
                    }
                    if (result & FLAG_STOP_LOOP) {
                        finfo->Release();
                        item->Release();
                        pl->EndUpdate();
                        return;
                    }
                }
                finfo->Release();
            }
            item->Release();
        }
    }
    pl->EndUpdate();
}

HWND Plugin::GetMainWindowHandle() {
    HWND handle = NULL;
    if (SUCCEEDED(m_messageDispatcher->Send(AIMP_MSG_PROPERTY_HWND, AIMP_MSG_PROPVALUE_GET, &handle))) {
        return handle;
    }
    return NULL;
}

std::wstring Plugin::Lang(const std::wstring &key, int part) {
    std::wstring ret;
    if (!m_muiService)
        return ret;

    IAIMPString *value = nullptr;
    if (part > -1) {
        if (SUCCEEDED(m_muiService->GetValuePart(AIMPString(key), part, &value))) {
            ret = value->GetData();
            value->Release();
        }
    } else {
        if (SUCCEEDED(m_muiService->GetValue(AIMPString(key), &value))) {
            ret = value->GetData();
            value->Release();
        }
    }
    return ret;
}

void Plugin::UpdatePlaylistMenu() {
    AimpMenu *playlistMenu = AimpMenu::Get(L"PlaylistsMenu");
    if (!playlistMenu) {
        if (AimpMenu *playlistManageMenu = AimpMenu::Get(AIMP_MENUID_PLAYER_PLAYLIST_MANAGE)) {
            playlistMenu = new AimpMenu(playlistManageMenu->Add(L"YouTube", [this](IAIMPMenuItem *) {
                if (!isConnected()) {
                    OptionsDialog::Connect([this] {
                        OptionsDialog *d = new OptionsDialog(this);
                        d->AddRef();
                        d->LoadProfileInfo([d] {
                            d->Notification(AIMP_SERVICE_OPTIONSDIALOG_NOTIFICATION_SAVE);
                            Timer::SingleShot(5000, [d] {
                                d->Release();
                            });
                        });
                        
                    });
                    return;
                }
            }, IDB_ICON, nullptr, L"PlaylistsMenu"));
            delete playlistManageMenu;
        }
    }
    if (playlistMenu) {
        playlistMenu->Clear();
        for (auto &x : Config::UserPlaylists) {
            playlistMenu->Add(x.Title, [this,&x](IAIMPMenuItem *) { YouTubeAPI::LoadUserPlaylist(x); })->Release();
        }
        delete playlistMenu;
    }

    AimpMenu *contextMenu = AimpMenu::Get(L"ContextMenu");
    if (!contextMenu) {
        if (AimpMenu *itemContextMenu = AimpMenu::Get(AIMP_MENUID_PLAYER_PLAYLIST_CONTEXT_FUNCTIONS)) {
            contextMenu = new AimpMenu(itemContextMenu->Add(Lang(L"YouTube.Menu\\AddTo"), nullptr, IDB_ICON, [this](IAIMPMenuItem *item) {
                int valid = 0;
                ForSelectedTracks([&valid](IAIMPPlaylist *, IAIMPPlaylistItem *, const std::wstring &id) -> int {
                    if (!id.empty()) valid++;
                    return 0;
                });

                item->SetValueAsInt32(AIMP_MENUITEM_PROPID_VISIBLE, valid > 0);
            }, L"ContextMenu"));
            delete itemContextMenu;
        }
    }
    if (contextMenu) {
        contextMenu->Clear();
        for (auto &x : Config::UserPlaylists) {
            if (x.CanModify) {
                contextMenu->Add(x.Title, [this, &x](IAIMPMenuItem *) {
                    ForSelectedTracks([&x](IAIMPPlaylist *, IAIMPPlaylistItem *, const std::wstring &id) -> int {
                        if (!id.empty()) {
                            YouTubeAPI::AddToPlaylist(x, id);
                        }
                        return 0;
                    });
                })->Release();
            }
        }
        delete contextMenu;
    }

    // REMOVE ITEMS
    contextMenu = AimpMenu::Get(L"RemoveContextMenu");
    if (!contextMenu) {
        if (AimpMenu *itemContextMenu = AimpMenu::Get(AIMP_MENUID_PLAYER_PLAYLIST_CONTEXT_FUNCTIONS)) {
            contextMenu = new AimpMenu(itemContextMenu->Add(Lang(L"YouTube.Menu\\RemoveFrom"), nullptr, IDB_ICON, [this](IAIMPMenuItem *item) {
                int valid = 0;
                ForSelectedTracks([&valid](IAIMPPlaylist *, IAIMPPlaylistItem *, const std::wstring &id) -> int {
                    if (!id.empty()) {
                        for (auto &x : Config::UserPlaylists) {
                            for (auto &item : x.Items) {
                                if (id == item) {
                                    valid++;
                                    return 0;
                                }
                            }
                        }
                    }
                    return 0;
                });

                item->SetValueAsInt32(AIMP_MENUITEM_PROPID_VISIBLE, valid > 0);
            }, L"RemoveContextMenu"));
            delete itemContextMenu;
        }
    }
    if (contextMenu) {
        contextMenu->Clear();
        for (auto &x : Config::UserPlaylists) {
            if (x.CanModify) {
                contextMenu->Add(x.Title, [this, &x](IAIMPMenuItem *) {
                    ForSelectedTracks([&x](IAIMPPlaylist *, IAIMPPlaylistItem *, const std::wstring &id) -> int {
                        if (!id.empty()) {
                            YouTubeAPI::RemoveFromPlaylist(x, id);
                        }
                        return 0;
                    });
                }, 0, [this, &x](IAIMPMenuItem *item) {
                    int valid = 0;
                    ForSelectedTracks([&valid, &x](IAIMPPlaylist *, IAIMPPlaylistItem *, const std::wstring &id) -> int {
                        if (!id.empty()) {
                            for (auto &item : x.Items) {
                                if (id == item) {
                                    valid++;
                                    return 0;
                                }
                            }
                        }
                        return 0;
                    });

                    item->SetValueAsInt32(AIMP_MENUITEM_PROPID_VISIBLE, valid > 0);
                })->Release();
            }
        }
        delete contextMenu;
    }
}

std::wstring Plugin::getAccessToken() {
    if (!m_refreshToken.empty() && m_tokenExpireTime - 30 <= std::time(nullptr))
        refreshAccessToken(true);
    return m_accessToken;
}

void Plugin::setAccessToken(const std::wstring &accessToken, const std::wstring &refreshToken, int expires_in) {
    m_accessToken = accessToken;
    m_refreshToken = refreshToken;
    m_tokenExpireTime = std::time(nullptr) + expires_in;
    Timer::SingleShot((expires_in - 30) * 1000, [this] { refreshAccessToken(); });

    Config::SetString(L"AccessToken", m_accessToken);
    Config::SetString(L"RefreshToken", m_refreshToken);
    Config::SetInt64(L"TokenExpires", m_tokenExpireTime);
}

void Plugin::refreshAccessToken(bool synchrounous) {
    if (m_refreshToken.empty())
        return;

    std::string post("client_id=" CLIENT_ID "&client_secret=" CLIENT_SECRET "&grant_type=refresh_token&refresh_token=" + Tools::ToString(m_refreshToken));

    AimpHTTP::Post(L"https://accounts.google.com/o/oauth2/token", post, [this](unsigned char *data, unsigned int size) {
        rapidjson::Document d;
        d.Parse(reinterpret_cast<char *>(data));
        if (d.IsObject() && d.HasMember("access_token")) {
            m_accessToken = Tools::ToWString(d["access_token"]);
            m_tokenExpireTime = std::time(nullptr) + d["expires_in"].GetUint();

            Config::SetString(L"AccessToken", m_accessToken);
            Config::SetInt64(L"TokenExpires", m_tokenExpireTime);

            Timer::SingleShot((d["expires_in"].GetUint() - 30) * 1000, [this] { refreshAccessToken(); });
        }
    }, synchrounous);
}
