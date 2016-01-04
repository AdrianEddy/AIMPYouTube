#include "YouTubeAPI.h"

#include "AIMPYouTube.h"
#include "AimpHTTP.h"
#include "SDK/apiFileManager.h"
#include "SDK/apiPlaylists.h"
#include "AIMPString.h"
#include "Tools.h"
#include "Timer.h"
#include <Strsafe.h>
#include <string>
#include <set>
#include <map>

YouTubeAPI::DecoderMap YouTubeAPI::SigDecoder;

void YouTubeAPI::AddFromJson(IAIMPPlaylist *playlist, const rapidjson::Value &d, LoadingState *state) {
    if (!playlist || !state)
        return;

    int insertAt = state->InsertPos;
    if (insertAt >= 0)
        insertAt += state->AdditionalPos;

    IAIMPFileInfo *file_info = nullptr;
    if (Plugin::instance()->core()->CreateObject(IID_IAIMPFileInfo, reinterpret_cast<void **>(&file_info)) == S_OK) {
        auto processItem = [&](const std::wstring &pid, const rapidjson::Value &item) {
            if (!item.HasMember("title"))
                return;

            std::wstring final_title = Tools::ToWString(item["title"]);
            if (final_title == L"Deleted video" || final_title == L"Private video")
                return;

            std::wstring trackId;
            if (item.HasMember("resourceId")) {
                trackId = Tools::ToWString(item["resourceId"]["videoId"]);
            } else {
                trackId = pid;
            }
            if (state->TrackIds.find(trackId) != state->TrackIds.end()) {
                // Already added earlier
                if (insertAt >= 0 && !(state->Flags & LoadingState::IgnoreExistingPosition)) {
                    insertAt++;
                    state->InsertPos++;
                    if (state->Flags & LoadingState::UpdateAdditionalPos)
                        state->AdditionalPos++;
                }
                return;
            }

            if (Config::TrackExclusions.find(trackId) != Config::TrackExclusions.end())
                return; // Track excluded

            state->TrackIds.insert(trackId);
            if (state->PlaylistToUpdate) {
                state->PlaylistToUpdate->Items.insert(trackId);
            }

            std::wstring final_url = L"https://www.youtube.com/watch?v=" + trackId;
            file_info->SetValueAsObject(AIMP_FILEINFO_PROPID_FILENAME, AIMPString(final_url));

            if (!state->ReferenceName.empty()) {
                file_info->SetValueAsObject(AIMP_FILEINFO_PROPID_ALBUM, AIMPString(state->ReferenceName));
            }
            if (item.HasMember("channelTitle") && (state->Flags & LoadingState::AddChannelTitle)) {
                file_info->SetValueAsObject(AIMP_FILEINFO_PROPID_ARTIST, AIMPString(item["channelTitle"]));
            }

            AIMPString title(final_title);
            // TODO
            /*int64_t videoDuration = 0;
            if (Config::GetInt32(L"AddDurationToTitle", 0)) {
                double duration = videoDuration / 1000.0;
                wchar_t buf[128];
                unsigned int hours = floor(duration / 3600.0);
                if (hours > 0) {
                    swprintf_s(buf, L" (%d:%02d:%02d)", hours, (uint32_t)floor(fmod(duration, 3600.0) / 60.0), (uint32_t)floor(fmod(duration, 60.0)));
                } else {
                    swprintf_s(buf, L" (%d:%02d)", (uint32_t)floor(fmod(duration, 3600.0) / 60.0), (uint32_t)floor(fmod(duration, 60.0)));
                }
                title->Add2(buf, wcslen(buf));
            }
            file_info->SetValueAsFloat(AIMP_FILEINFO_PROPID_DURATION, videoDuration / 1000.0);*/

            file_info->SetValueAsObject(AIMP_FILEINFO_PROPID_TITLE, title);

            const DWORD flags = AIMP_PLAYLIST_ADD_FLAGS_FILEINFO | AIMP_PLAYLIST_ADD_FLAGS_NOCHECKFORMAT | AIMP_PLAYLIST_ADD_FLAGS_NOEXPAND | AIMP_PLAYLIST_ADD_FLAGS_NOASYNC;
            if (SUCCEEDED(playlist->Add(file_info, flags, insertAt))) {
                state->AddedItems++;
                if (insertAt >= 0) {
                    insertAt++;
                    state->InsertPos++;
                    if (state->Flags & LoadingState::UpdateAdditionalPos)
                        state->AdditionalPos++;
                }
            }

            DebugA("Adding %s\n", item["title"].GetString());
        };

        if (d.IsArray()) {
            for (auto x = d.Begin(), e = d.End(); x != e; x++) {
                const rapidjson::Value *px = &(*x);
                if (!px->IsObject() || !px->HasMember("snippet"))
                    continue;

                processItem((*px).HasMember("id") ? Tools::ToWString((*px)["id"]) : L"", (*px)["snippet"]);
            }
        } else if (d.IsObject() && d.HasMember("snippet")) {
            processItem(d.HasMember("id")? Tools::ToWString(d["id"]) : L"", d["snippet"]);
        } else if (d.IsObject()) {
            processItem(L"", d);
        }
        file_info->Release();
    }
}

void YouTubeAPI::LoadFromUrl(std::wstring url, IAIMPPlaylist *playlist, LoadingState *state, std::function<void()> finishCallback) {
    if (!playlist || !state)
        return;

    std::wstring reqUrl(url);
    if (reqUrl.find(L'?') == std::wstring::npos) {
        reqUrl += L'?';
    } else {
        reqUrl += L'&';
    }
    reqUrl += L"key=" TEXT(APP_KEY);
    if (Plugin::instance()->isConnected())
        reqUrl += L"\r\nAuthorization: Bearer " + Plugin::instance()->getAccessToken();

    AimpHTTP::Get(reqUrl, [playlist, state, finishCallback, url](unsigned char *data, int size) {
        rapidjson::Document d;
        d.Parse(reinterpret_cast<const char *>(data));

        playlist->BeginUpdate();
        if (d.IsObject() && d.HasMember("items") && d["items"].IsArray() && d["items"].Size() > 0 && d["items"][0].HasMember("contentDetails")) {
            const rapidjson::Value &i = d["items"][0]["contentDetails"]["relatedPlaylists"];
            std::wstring uploads = Tools::ToWString(i["uploads"]);
            /*std::wstring favorites = Tools::ToWString(i["favorites"]);
            std::wstring likes = Tools::ToWString(i["likes"]);
            std::wstring watchLater = Tools::ToWString(i["watchLater"]);*/
            std::wstring userName = Tools::ToWString(d["items"][0]["snippet"]["localized"]["title"]);
            IAIMPPropertyList *plProp = nullptr;
            if (SUCCEEDED(playlist->QueryInterface(IID_IAIMPPropertyList, reinterpret_cast<void **>(&plProp)))) {
                plProp->SetValueAsObject(AIMP_PLAYLIST_PROPID_NAME, AIMPString(userName));
                plProp->Release();
            }
            state->ReferenceName = userName;

            LoadFromUrl(L"https://content.googleapis.com/youtube/v3/playlistItems?part=snippet&maxResults=50&playlistId=" + uploads +
                        L"&fields=items%2Fsnippet%2Ckind%2CnextPageToken%2CpageInfo%2CtokenPagination", playlist, state, finishCallback);

            playlist->EndUpdate();
            return;
        } else if (d.IsObject() && d.HasMember("items")) {
            AddFromJson(playlist, d["items"], state);
        } else {
            AddFromJson(playlist, d, state);
        }
        playlist->EndUpdate();

        bool processNextPage = d.IsObject() && d.HasMember("nextPageToken");

        if ((state->Flags & LoadingState::IgnoreNextPage) ||
            (Config::GetInt32(L"LimitUserStream", 0) && state->AddedItems >= Config::GetInt32(L"LimitUserStreamValue", 5000))) {
            processNextPage = false;
        }

        if (processNextPage) {
            std::wstring next_url(url);
            std::size_t pos = 0;
            if ((pos = next_url.find(L"&pageToken")) != std::wstring::npos)
                next_url = next_url.substr(0, pos);

            LoadFromUrl(next_url + L"&pageToken=" + Tools::ToWString(d["nextPageToken"]), playlist, state, finishCallback);
        } else if (!state->PendingUrls.empty()) {
            const LoadingState::PendingUrl &pl = state->PendingUrls.front();
            if (!pl.Title.empty()) {
                state->ReferenceName = pl.Title;
            }
            if (pl.PlaylistPosition > -3) { // -3 = don't change
                state->InsertPos = pl.PlaylistPosition;
                state->Flags = LoadingState::UpdateAdditionalPos | LoadingState::IgnoreExistingPosition;
            }

            LoadFromUrl(pl.Url, playlist, state, finishCallback);
            state->PendingUrls.pop();
        } else {
            // Finished
            if (state->PlaylistToUpdate) {
                Config::SaveExtendedConfig();
            }
            playlist->Release();
            delete state;
            if (finishCallback)
                finishCallback();
        }
    });
}

void YouTubeAPI::LoadUserPlaylist(Config::Playlist &playlist) {
    if (!Plugin::instance()->isConnected()) {
        OptionsDialog::Connect([&playlist] { LoadUserPlaylist(playlist); });
        return;
    }
    std::wstring playlistId = playlist.ID;
    std::wstring uname = Config::GetString(L"UserYTName");
    std::wstring playlistName(uname + L" - " + playlist.Title);
    std::wstring groupName(uname + L" - " + playlist.Title);

    IAIMPPlaylist *pl = Plugin::instance()->GetPlaylist(playlistName);

    LoadingState *state = new LoadingState();
    state->PlaylistToUpdate = &playlist;
    state->ReferenceName = groupName;
    GetExistingTrackIds(pl, state);

    std::wstring url(L"https://content.googleapis.com/youtube/v3/playlistItems?part=snippet&maxResults=50&playlistId=" + playlistId +
                     L"&fields=items%2Fsnippet%2Ckind%2CnextPageToken%2CpageInfo%2CtokenPagination");

    std::wstring plId;
    IAIMPPropertyList *plProp = nullptr;
    if (SUCCEEDED(pl->QueryInterface(IID_IAIMPPropertyList, reinterpret_cast<void **>(&plProp)))) {
        IAIMPString *str = nullptr;
        if (SUCCEEDED(plProp->GetValueAsObject(AIMP_PLAYLIST_PROPID_ID, IID_IAIMPString, reinterpret_cast<void **>(&str)))) {
            plId = str->GetData();
            str->Release();
        }
        plProp->Release();
    }
    playlist.AIMPPlaylistId = plId;

    if (Config::GetInt32(L"MonitorUserPlaylists", 1)) {
        auto find = [&](const Config::MonitorUrl &p) -> bool { return p.PlaylistID == plId && p.URL == url; };
        if (std::find_if(Config::MonitorUrls.begin(), Config::MonitorUrls.end(), find) == Config::MonitorUrls.end()) {
            Config::MonitorUrls.push_back({ url, plId, state->Flags, groupName });
        }
        Config::SaveExtendedConfig();
    }

    LoadFromUrl(url, pl, state);
}

void YouTubeAPI::GetExistingTrackIds(IAIMPPlaylist *pl, LoadingState *state) {
    if (!pl || !state)
        return;

    // Fetch current track ids from playlist
    Plugin::instance()->ForEveryItem(pl, [&](IAIMPPlaylistItem *, IAIMPFileInfo *, const std::wstring &id) -> int {
        if (!id.empty()) {
            state->TrackIds.insert(id);
        }
        return 0;
    });
}

void YouTubeAPI::ResolveUrl(const std::wstring &url, const std::wstring &playlistTitle, bool createPlaylist) {
    if (url.find(L"youtube.com") != std::wstring::npos || url.find(L"youtu.be") != std::wstring::npos) {
        std::wstring finalUrl;
        rapidjson::Value *addDirectly = nullptr;
        std::wstring plName;
        bool monitor = true;
        LoadingState *state = new LoadingState();
        std::set<std::wstring> toMonitor;
        std::wstring ytPlaylistId;

        if (url.find(L"/user/") != std::wstring::npos) {
            std::wstring id;
            std::wstring::size_type pos;
            if ((pos = url.find(L"/user/")) != std::wstring::npos) {
                id = url.c_str() + pos + 6;
            }
            if ((pos = id.find(L'/')) != std::wstring::npos)
                id.resize(pos);

            finalUrl = L"https://www.googleapis.com/youtube/v3/channels?part=contentDetails%2Csnippet&hl=" + Plugin::instance()->Lang(L"YouTube\\YouTubeLang") + L"&forUsername=" + id + L"&fields=items(contentDetails%2Csnippet)";
            plName = L"YouTube";
        } else if (url.find(L"/channel/") != std::wstring::npos) {
            std::wstring id;
            std::wstring::size_type pos;
            if ((pos = url.find(L"/channel/")) != std::wstring::npos) {
                id = url.c_str() + pos + 9;
            }
            if ((pos = id.find(L'/')) != std::wstring::npos)
                id.resize(pos);

            finalUrl = L"https://www.googleapis.com/youtube/v3/channels?part=contentDetails%2Csnippet&hl=" + Plugin::instance()->Lang(L"YouTube\\YouTubeLang") + L"&id=" + id + L"&fields=items(contentDetails%2Csnippet)";
            plName = L"YouTube";
        } else if (url.find(L"list=") != std::wstring::npos) {
            std::wstring id;
            std::wstring::size_type pos;
            if ((pos = url.find(L"?list=")) != std::wstring::npos) {
                id = url.c_str() + pos + 6;
            } else if ((pos = url.find(L"&list=")) != std::wstring::npos) {
                id = url.c_str() + pos + 6;
            }
            if ((pos = id.find(L'&')) != std::wstring::npos)
                id.resize(pos);

            finalUrl = L"https://content.googleapis.com/youtube/v3/playlistItems?part=snippet&maxResults=50&playlistId=" + id +
                       L"&fields=items%2Fsnippet%2Ckind%2CnextPageToken%2CpageInfo%2CtokenPagination";
            plName = L"YouTube";
            ytPlaylistId = id;
        } else if (url.find(L"watch?") != std::wstring::npos) {
            std::wstring id = Tools::TrackIdFromUrl(url);
            finalUrl = L"https://www.googleapis.com/youtube/v3/videos?part=snippet&hl=" + Plugin::instance()->Lang(L"YouTube\\YouTubeLang") + L"&id=" + id;
            plName = L"YouTube";
        } else if (url.find(L"youtu.be") != std::wstring::npos) {
            std::wstring id = Tools::TrackIdFromUrl(url);
            finalUrl = L"https://www.googleapis.com/youtube/v3/videos?part=snippet&hl=" + Plugin::instance()->Lang(L"YouTube\\YouTubeLang") + L"&id=" + id;
            plName = L"YouTube";
        }

        IAIMPPlaylist *pl = nullptr;
        std::wstring finalPlaylistName;
        if (createPlaylist) {
            finalPlaylistName = playlistTitle.empty() ? plName : playlistTitle;
            pl = Plugin::instance()->GetPlaylist(finalPlaylistName);
            if (!pl) {
                delete state;
                return;
            }
        } else {
            pl = Plugin::instance()->GetCurrentPlaylist();
            if (!pl) {
                delete state;
                return;
            }
        }

        std::wstring playlistId;
        IAIMPPropertyList *plProp = nullptr;
        if (SUCCEEDED(pl->QueryInterface(IID_IAIMPPropertyList, reinterpret_cast<void **>(&plProp)))) {
            IAIMPString *str = nullptr;
            if (SUCCEEDED(plProp->GetValueAsObject(AIMP_PLAYLIST_PROPID_ID, IID_IAIMPString, reinterpret_cast<void **>(&str)))) {
                playlistId = str->GetData();
                str->Release();
            }
            if (finalPlaylistName.empty()) {
                if (SUCCEEDED(plProp->GetValueAsObject(AIMP_PLAYLIST_PROPID_NAME, IID_IAIMPString, reinterpret_cast<void **>(&str)))) {
                    finalPlaylistName = str->GetData();
                    str->Release();
                }
            }
            plProp->Release();
        }
        if (!ytPlaylistId.empty()) {
            AimpHTTP::Get(L"https://www.googleapis.com/youtube/v3/playlists?part=snippet&hl=" + Plugin::instance()->Lang(L"YouTube\\YouTubeLang") + L"&id=" + ytPlaylistId + L"&key=" TEXT(APP_KEY), [pl](unsigned char *data, int size) {
                rapidjson::Document d;
                d.Parse(reinterpret_cast<const char *>(data));
                if (d.IsObject() && d.HasMember("items") && d["items"].IsArray() && d["items"].Size() > 0 && d["items"][0].HasMember("snippet")) {
                    rapidjson::Value &val = d["items"][0]["snippet"];
                    std::wstring channelName = Tools::ToWString(val["channelTitle"]);
                    std::wstring playlistTitle = Tools::ToWString(val["localized"]["title"]);
                    IAIMPPropertyList *plProp = nullptr;
                    if (SUCCEEDED(pl->QueryInterface(IID_IAIMPPropertyList, reinterpret_cast<void **>(&plProp)))) {
                        plProp->SetValueAsObject(AIMP_PLAYLIST_PROPID_NAME, AIMPString(channelName + L" - " + playlistTitle));
                        plProp->Release();
                    }
                }
            });
        }

        state->ReferenceName = plName;

        GetExistingTrackIds(pl, state);

        if (addDirectly) {
            AddFromJson(pl, *addDirectly, state);
            delete state;
        } else {
            LoadFromUrl(finalUrl, pl, state);
        }

        if (monitor) {
            toMonitor.insert(finalUrl);
            for (const auto &x : toMonitor) {
                auto find = [&](const Config::MonitorUrl &p) -> bool { return p.PlaylistID == playlistId && p.URL == x; };
                if (std::find_if(Config::MonitorUrls.begin(), Config::MonitorUrls.end(), find) == Config::MonitorUrls.end()) {
                    Config::MonitorUrls.push_back({ x, playlistId, state->Flags, plName });
                }
            }
            Config::SaveExtendedConfig();
        }
        return;
    }

    MessageBox(Plugin::instance()->GetMainWindowHandle(), Plugin::instance()->Lang(L"YouTube.Messages\\CantResolve").c_str(), Plugin::instance()->Lang(L"YouTube.Messages\\Error").c_str(), MB_OK | MB_ICONERROR);
}

std::wstring YouTubeAPI::GetStreamUrl(const std::wstring &id) {
    std::wstring stream_url;
    std::wstring url2(L"http://www.youtube.com/get_video_info?video_id=" + id + L"&el=detailpage&sts=16511");
    AimpHTTP::Get(url2, [&](unsigned char *data, int size) {
        if (char *streams = strstr((char *)data, "url_encoded_fmt_stream_map=")) {
            streams += 27;
            if (char *end = strchr(streams, '&'))
                *end = 0;
            std::string map = Tools::UrlDecode(streams);
            Tools::ReplaceString(",", "\"},{\"", map);
            Tools::ReplaceString("&", "\",\"", map);
            Tools::ReplaceString("=", "\":\"", map);
            map = "[{\"" + map + "\"}]";

            rapidjson::Document d;
            d.Parse(map.c_str());

            if (d.IsArray()) {
                for (auto x = d.Begin(), e = d.End(); x != e; x++) {
                    const rapidjson::Value &px = *x;
                    if (!px.IsObject())
                        continue;

                    if (px.HasMember("quality") && strcmp(px["quality"].GetString(), "medium") == 0 && !px.HasMember("stereo3d") && strstr(px["type"].GetString(), "mp4") != nullptr) {
                        std::string stream = Tools::UrlDecode(px["url"].GetString());
                        if (px.HasMember("s")) {
                            std::string s = px["s"].GetString();
                            YouTubeAPI::DecodeSignature(s);
                            stream += "&signature=" + s;
                        }

                        stream_url = Tools::ToWString(stream);
                        return;
                    }
                }
            }
        }
    }, true);
    return stream_url;
}

void YouTubeAPI::LoadSignatureDecoder() {
    // TODO: http://en.cppreference.com/w/cpp/regex/regex_match

    std::wstring ua(L"Mozilla/5.0 (Windows NT 6.3; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/44.0.2403.157 Safari/537.36");
    AimpHTTP::Get(L"https://www.youtube.com/\r\nUser-Agent: " + ua, [](unsigned char *data, int size) {
        if (char *playerurl = strstr((char *)data, "\"js\":\"")) {
            playerurl += 6;
            if (char *end = strchr(playerurl, '"'))
                *end = 0;
            std::string player = playerurl;
            Tools::ReplaceString("\\/", "/", player);
            if (player.find("http") == std::string::npos)
                player = "http:" + player;

            AimpHTTP::Get(Tools::ToWString(player), [](unsigned char *rawData, int size) {
                std::string data(reinterpret_cast<char *>(rawData), size);
                Tools::ReplaceString("\n", "", data);
                Tools::ReplaceString("\r", "", data);

                std::size_t funcname;
                if ((funcname = data.find(".set(\"signature\",")) != std::string::npos) {
                    funcname += 17;
                    std::size_t end;
                    if ((end = data.find('(', funcname)) != std::string::npos) {
                        std::string fname(data.substr(funcname, end - funcname));
                        std::string funcsig = "function " + fname;
                        std::string funcsig2 = fname + "=function";
                        int sigLen = 0;
                        std::size_t funcdef = data.find(funcsig);
                        if (funcdef != std::string::npos) {
                            sigLen = funcsig.size() + 4;
                        } else {
                            funcdef = data.find(funcsig2);
                            sigLen = funcsig2.size() + 4;
                        }
                        if (funcdef != std::string::npos) {
                            size_t mutatorObject = data.find("split(\"\");", funcdef) + 10;
                            std::string mutatorObjectName(data.substr(mutatorObject, data.find('.', mutatorObject) - mutatorObject));
                            mutatorObjectName = "var " + mutatorObjectName;
                            
                            size_t mutatorObjectStart;
                            if ((mutatorObjectStart = data.find(mutatorObjectName)) != std::string::npos) {
                                mutatorObjectStart += mutatorObjectName.length() + 2;
                                size_t mutatorObjectEnd = data.find("};", mutatorObjectStart);
                                end = data.find("join(\"\")", funcdef);
                                funcdef += sigLen;

                                std::map<std::string, std::function<void(std::string &s, int param)>> mutators;
                                Tools::SplitString(data.substr(mutatorObjectStart, mutatorObjectEnd - mutatorObjectStart), "},", [&](std::string token) {
                                    token = Tools::Trim(token);
                                    std::string name(token.substr(0, 2));
                                    if (token.find("var ") != std::string::npos)
                                        mutators[name] = [](std::string &s, int param) { std::swap(s[0], s[param]); };

                                    if (token.find("splice") != std::string::npos)
                                        mutators[name] = [](std::string &s, int param) { s.erase(0, param); };

                                    if (token.find("reverse") != std::string::npos)
                                        mutators[name] = [](std::string &s, int param) { std::reverse(s.begin(), s.end()); };
                                });

                                Tools::SplitString(data.substr(funcdef, end - funcdef), ";", [&](std::string token) {
                                    token = Tools::Trim(token);
                                    if (token.find("split") != std::string::npos || token.find("return") != std::string::npos)
                                        return;

                                    std::string mutator(token.substr(3, 2));
                                    if (char *params = strchr((char *)token.c_str(), ',')) {
                                        if (char *end = strchr(params, ')')) *end = 0;
                                        int param = std::stoi(params + 1);
                                        YouTubeAPI::SigDecoder.push_back({ mutators[mutator], param });
                                    }
                                });
                            }
                        }
                    }
                }
            }, true);
        }
    }, true);
}

void YouTubeAPI::AddToPlaylist(Config::Playlist &pl, const std::wstring &trackId) {
    if (!Plugin::instance()->isConnected()) {
        OptionsDialog::Connect([&pl, trackId] { AddToPlaylist(pl, trackId); });
        return;
    }
    std::string postData("{"
        "\"snippet\": {"
            "\"playlistId\": \"" + Tools::ToString(pl.ID) + "\","
            "\"resourceId\": {\"videoId\": \""+Tools::ToString(trackId)+"\", \"kind\": \"youtube#video\" }"
        "}"
    "}");
    std::wstring headers(L"\r\nContent-Type: application/json"
                         L"\r\nAuthorization: Bearer " + Plugin::instance()->getAccessToken());

    AimpHTTP::Post(L"https://www.googleapis.com/youtube/v3/playlistItems?part=snippet" + headers, postData, [&pl, trackId](unsigned char *data, int size) {
        if (strstr(reinterpret_cast<char *>(data), "youtube#playlistItem")) {
            pl.Items.insert(trackId);
            Config::SaveExtendedConfig();

            Timer::SingleShot(0, Plugin::MonitorCallback);
        }
    });
}

void YouTubeAPI::RemoveFromPlaylist(Config::Playlist &pl, const std::wstring &trackId) {
    if (!Plugin::instance()->isConnected()) {
        OptionsDialog::Connect([&pl, trackId] { RemoveFromPlaylist(pl, trackId); });
        return;
    }
    std::wstring headers(L"\r\nAuthorization: Bearer " + Plugin::instance()->getAccessToken());

    AimpHTTP::Get(L"https://content.googleapis.com/youtube/v3/playlistItems?part=id&videoId=" + trackId + L"&playlistId=" + pl.ID +
                  L"&fields=items%2Fid" + headers, [&pl, trackId, headers](unsigned char *data, int size) {
        rapidjson::Document d;
        d.Parse(reinterpret_cast<const char *>(data));

        if (d.IsObject() && d.HasMember("items") && d["items"].IsArray() && d["items"].Size() > 0) {
            std::wstring url(L"https://www.googleapis.com/youtube/v3/playlistItems?id=" + Tools::ToWString(d["items"][0]["id"]));

            url += L"\r\nX-HTTP-Method-Override: DELETE";
            url += headers;

            AimpHTTP::Post(url, std::string(), [&pl, trackId](unsigned char *data, int size) {
                if (strlen(reinterpret_cast<char *>(data)) == 0) {
                    // removed correctly
                    pl.Items.erase(trackId);
                    Config::SaveExtendedConfig();

                    if (IAIMPPlaylist *playlist = Plugin::instance()->GetPlaylistById(pl.AIMPPlaylistId)) {
                        Plugin::instance()->ForEveryItem(playlist, [&trackId](IAIMPPlaylistItem *, IAIMPFileInfo *, const std::wstring &id) {
                            if (!id.empty() && id == trackId) {
                                return Plugin::FLAG_DELETE_ITEM | Plugin::FLAG_STOP_LOOP;
                            }
                            return 0;
                        });
                        playlist->Release();
                    }
                }
            });
        }
    });
}

