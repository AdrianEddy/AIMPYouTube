#include "MessageHook.h"

#include "Tools.h"
#include "SDK/apiPlaylists.h"
#include "SDK/apiPlayer.h"
#include "AIMPYouTube.h"
#include "YouTubeAPI.h"

MessageHook::MessageHook(Plugin *pl) : m_plugin(pl) {
    
}

MessageHook::~MessageHook() {
    
}

void WINAPI MessageHook::CoreMessage(DWORD AMessage, int AParam1, void *AParam2, HRESULT *AResult) {
    if (AMessage == AIMP_MSG_CMD_PLS_DELETE_PLAYING_FROM_HDD) {
        IAIMPString *url = nullptr;
        IAIMPPlaylistItem *currentTrack = m_plugin->GetCurrentTrack();
        if (SUCCEEDED(currentTrack->GetValueAsObject(AIMP_PLAYLISTITEM_PROPID_FILENAME, IID_IAIMPString, reinterpret_cast<void **>(&url)))) {
            std::wstring id = Tools::TrackIdFromUrl(url->GetData());
            url->Release();
            if (!id.empty()) {
                for (auto &x : Config::UserPlaylists) {
                    for (auto &xx : x.Items) {
                        if (xx == id) {
                            x.Items.erase(id);

                            if (IAIMPPlaylist *playlist = Plugin::instance()->GetPlaylistById(x.AIMPPlaylistId)) {
                                Plugin::instance()->ForEveryItem(playlist, [&id](IAIMPPlaylistItem *, IAIMPFileInfo *, const std::wstring &itemid) {
                                    if (!itemid.empty() && itemid == id) {
                                        return Plugin::FLAG_DELETE_ITEM | Plugin::FLAG_STOP_LOOP;
                                    }
                                    return 0;
                                });
                                playlist->Release();
                            }
                            break;
                        }
                    }
                }
                Config::TrackExclusions.insert(id);
                Config::SaveExtendedConfig();
                IAIMPPlaylist *parent = nullptr;
                if (SUCCEEDED(currentTrack->GetValueAsObject(AIMP_PLAYLISTITEM_PROPID_PLAYLIST, IID_IAIMPPlaylist, reinterpret_cast<void **>(&parent)))) {
                    parent->Delete(currentTrack);
                    parent->Release();
                }
                currentTrack->Release();

                IAIMPServicePlayer *player = nullptr;
                if (SUCCEEDED(m_plugin->m_core->QueryInterface(IID_IAIMPServicePlayer, reinterpret_cast<void **>(&player)))) {
                    player->GoToNext();
                    player->Release();
                }
                *AResult = S_OK;
                return;
            }
        }
    }
    if (AMessage == AIMP_MSG_CMD_BOOKMARKS_ADD) {
        IAIMPString *url = nullptr;
        IAIMPPlaylistItem *currentTrack = m_plugin->GetCurrentTrack();
        if (SUCCEEDED(currentTrack->GetValueAsObject(AIMP_PLAYLISTITEM_PROPID_FILENAME, IID_IAIMPString, reinterpret_cast<void **>(&url)))) {
            std::wstring id = Tools::TrackIdFromUrl(url->GetData());
            url->Release();
            currentTrack->Release();
            if (!id.empty()) {
                for (auto &x : Config::UserPlaylists) {
                    if (x.ReferenceName == std::wstring(L"Favorites")) {
                        YouTubeAPI::AddToPlaylist(x, id);
                        break;
                    }
                }
                return;
            }
        }
    }

#if 0
#   define STRINGIFY(name) case name: DebugA(#name " %d %d\n", AParam1, AParam2); break;
    
    switch (AMessage) {
        STRINGIFY(AIMP_MSG_CMD_BASE)
        STRINGIFY(AIMP_MSG_CMD_STATE_GET)
        STRINGIFY(AIMP_MSG_CMD_QFI_PLAYBACK_TRACK)
        STRINGIFY(AIMP_MSG_CMD_SHOW_NOTIFICATION)
        STRINGIFY(AIMP_MSG_CMD_TOGGLE_PARTREPEAT)
        STRINGIFY(AIMP_MSG_CMD_ABOUT)
        STRINGIFY(AIMP_MSG_CMD_OPTIONS)
        STRINGIFY(AIMP_MSG_CMD_PLUGINS)
        STRINGIFY(AIMP_MSG_CMD_QUIT)
        STRINGIFY(AIMP_MSG_CMD_SCHEDULER)
        STRINGIFY(AIMP_MSG_CMD_VISUAL_NEXT)
        STRINGIFY(AIMP_MSG_CMD_VISUAL_PREV)
        STRINGIFY(AIMP_MSG_CMD_PLAY)
        STRINGIFY(AIMP_MSG_CMD_PLAYPAUSE)
        STRINGIFY(AIMP_MSG_CMD_PLAY_PREV_PLAYLIST)
        STRINGIFY(AIMP_MSG_CMD_PAUSE)
        STRINGIFY(AIMP_MSG_CMD_STOP)
        STRINGIFY(AIMP_MSG_CMD_NEXT)
        STRINGIFY(AIMP_MSG_CMD_PREV)
        STRINGIFY(AIMP_MSG_CMD_OPEN_FILES)
        STRINGIFY(AIMP_MSG_CMD_OPEN_FOLDERS)
        STRINGIFY(AIMP_MSG_CMD_OPEN_PLAYLISTS)
        STRINGIFY(AIMP_MSG_CMD_SAVE_PLAYLISTS)
        STRINGIFY(AIMP_MSG_CMD_BOOKMARKS)
        STRINGIFY(AIMP_MSG_CMD_BOOKMARKS_ADD)
        STRINGIFY(AIMP_MSG_CMD_PLS_RESCAN)
        STRINGIFY(AIMP_MSG_CMD_PLS_FOCUS_PLAYABLE)
        STRINGIFY(AIMP_MSG_CMD_PLS_DELETE_ALL)
        STRINGIFY(AIMP_MSG_CMD_PLS_DELETE_NON_EXISTS)
        STRINGIFY(AIMP_MSG_CMD_PLS_DELETE_NON_SELECTED)
        STRINGIFY(AIMP_MSG_CMD_PLS_DELETE_PLAYING_FROM_HDD)
        STRINGIFY(AIMP_MSG_CMD_PLS_DELETE_SELECTED)
        STRINGIFY(AIMP_MSG_CMD_PLS_DELETE_SELECTED_FROM_HDD)
        STRINGIFY(AIMP_MSG_CMD_PLS_DELETE_SWITCHEDOFF)
        STRINGIFY(AIMP_MSG_CMD_PLS_DELETE_SWITCHEDOFF_FROM_HDD)
        STRINGIFY(AIMP_MSG_CMD_PLS_DELETE_DUPLICATES)
        STRINGIFY(AIMP_MSG_CMD_PLS_SORT_BY_ARTIST)
        STRINGIFY(AIMP_MSG_CMD_PLS_SORT_BY_TITLE)
        STRINGIFY(AIMP_MSG_CMD_PLS_SORT_BY_PATH)
        STRINGIFY(AIMP_MSG_CMD_PLS_SORT_BY_DURATION)
        STRINGIFY(AIMP_MSG_CMD_PLS_SORT_RANDOMIZE)
        STRINGIFY(AIMP_MSG_CMD_PLS_SORT_INVERT)
        STRINGIFY(AIMP_MSG_CMD_PLS_SWITCH_ON)
        STRINGIFY(AIMP_MSG_CMD_PLS_SWITCH_OFF)
        STRINGIFY(AIMP_MSG_CMD_ADD_FILES)
        STRINGIFY(AIMP_MSG_CMD_ADD_FOLDERS)
        STRINGIFY(AIMP_MSG_CMD_ADD_PLAYLISTS)
        STRINGIFY(AIMP_MSG_CMD_ADD_URL)
        STRINGIFY(AIMP_MSG_CMD_QTE_PLAYABLE_TRACK)
        STRINGIFY(AIMP_MSG_CMD_SEARCH)
        STRINGIFY(AIMP_MSG_CMD_DSPMANAGER)
        STRINGIFY(AIMP_MSG_CMD_PLS_RELOAD_FROM_PREIMAGE)
        STRINGIFY(AIMP_MSG_CMD_VISUAL_START)
        STRINGIFY(AIMP_MSG_CMD_VISUAL_STOP)
        STRINGIFY(AIMP_MSG_PROPERTY_BASE)
        STRINGIFY(AIMP_MSG_PROPERTY_VOLUME)
        STRINGIFY(AIMP_MSG_PROPERTY_MUTE)
        STRINGIFY(AIMP_MSG_PROPERTY_BALANCE)
        STRINGIFY(AIMP_MSG_PROPERTY_CHORUS)
        STRINGIFY(AIMP_MSG_PROPERTY_ECHO)
        STRINGIFY(AIMP_MSG_PROPERTY_ENHANCER)
        STRINGIFY(AIMP_MSG_PROPERTY_FLANGER)
        STRINGIFY(AIMP_MSG_PROPERTY_REVERB)
        STRINGIFY(AIMP_MSG_PROPERTY_PITCH)
        STRINGIFY(AIMP_MSG_PROPERTY_SPEED)
        STRINGIFY(AIMP_MSG_PROPERTY_TEMPO)
        STRINGIFY(AIMP_MSG_PROPERTY_TRUEBASS)
        STRINGIFY(AIMP_MSG_PROPERTY_PREAMP)
        STRINGIFY(AIMP_MSG_PROPERTY_EQUALIZER)
        STRINGIFY(AIMP_MSG_PROPERTY_EQUALIZER_BAND)
        STRINGIFY(AIMP_MSG_PROPERTY_PLAYER_STATE)
        //STRINGIFY(AIMP_MSG_PROPERTY_PLAYER_POSITION)
        //STRINGIFY(AIMP_MSG_PROPERTY_PLAYER_DURATION)
        STRINGIFY(AIMP_MSG_PROPERTY_PARTREPEAT)
        STRINGIFY(AIMP_MSG_PROPERTY_REPEAT)
        STRINGIFY(AIMP_MSG_PROPERTY_SHUFFLE)
        STRINGIFY(AIMP_MSG_PROPERTY_HWND)
        STRINGIFY(AIMP_MSG_PROPERTY_STAYONTOP)
        STRINGIFY(AIMP_MSG_PROPERTY_REVERSETIME)
        STRINGIFY(AIMP_MSG_PROPERTY_MINIMIZED_TO_TRAY)
        STRINGIFY(AIMP_MSG_PROPERTY_REPEAT_SINGLE_FILE_PLAYLISTS)
        STRINGIFY(AIMP_MSG_PROPERTY_ACTION_ON_END_OF_PLAYLIST)
        STRINGIFY(AIMP_MSG_PROPERTY_STOP_AFTER_TRACK)
        STRINGIFY(AIMP_MSG_PROPERTY_RADIOCAP)
        STRINGIFY(AIMP_MSG_PROPERTY_LOADED)
        STRINGIFY(AIMP_MSG_PROPERTY_VISUAL_FULLSCREEN)
        STRINGIFY(AIMP_MSG_EVENT_BASE)
        STRINGIFY(AIMP_MSG_EVENT_CMD_STATE)
        STRINGIFY(AIMP_MSG_EVENT_OPTIONS)
        STRINGIFY(AIMP_MSG_EVENT_STREAM_START)
        STRINGIFY(AIMP_MSG_EVENT_STREAM_START_SUBTRACK)
        STRINGIFY(AIMP_MSG_EVENT_STREAM_END)
        STRINGIFY(AIMP_MSG_EVENT_PLAYER_STATE)
        STRINGIFY(AIMP_MSG_EVENT_PROPERTY_VALUE)
        STRINGIFY(AIMP_MSG_EVENT_OPTIONS_FRAME_LIST)
        STRINGIFY(AIMP_MSG_EVENT_OPTIONS_FRAME_MODIFIED)
        STRINGIFY(AIMP_MSG_EVENT_VISUAL_PLUGIN)
        STRINGIFY(AIMP_MSG_EVENT_FILEMARK)
        STRINGIFY(AIMP_MSG_EVENT_STATISTICS_CHANGED)
        STRINGIFY(AIMP_MSG_EVENT_SKIN)
        //STRINGIFY(AIMP_MSG_EVENT_PLAYER_UPDATE_POSITION)
        STRINGIFY(AIMP_MSG_EVENT_LANGUAGE)
        STRINGIFY(AIMP_MSG_EVENT_TERMINATING)
        STRINGIFY(AIMP_MSG_EVENT_PLAYABLE_FILE_INFO)
        //default: DebugA("%d %d %d\n", AMessage, AParam1, AParam2); break;
    }
#endif
}
