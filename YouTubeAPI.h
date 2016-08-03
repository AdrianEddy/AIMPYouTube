#pragma once

#include "rapidjson/document.h"
#include <queue>
#include <unordered_set>
#include <cstdint>
#include <functional>
#include <windows.h>
#include "Config.h"
#include <memory>

class IAIMPPlaylist;
class IAIMPPlaylistItem;

class YouTubeAPI {
    typedef std::vector<std::pair<std::function<void(std::string &s, int param)>, int>> DecoderMap;
public:
    struct LoadingState {
        struct PendingUrl {
            std::wstring Title;
            std::wstring Url;
            int PlaylistPosition;
        };
        enum LoadingFlags {
            None                   = 0x00,
            UpdateAdditionalPos    = 0x01,
            AddChannelTitle        = 0x02,
            IgnoreExistingPosition = 0x04,
            IgnoreNextPage         = 0x08
        };
        std::unordered_set<std::wstring> TrackIds;
        std::queue<PendingUrl> PendingUrls;
        std::wstring ReferenceName;
        Config::Playlist *PlaylistToUpdate;
        int AdditionalPos;
        int InsertPos;
        int Offset;
        int AddedItems;
        int Flags;
        LoadingState() : AdditionalPos(0), InsertPos(0), Offset(0), AddedItems(0), PlaylistToUpdate(nullptr), Flags(None) {}
    };

    static std::wstring GetStreamUrl(const std::wstring &id);

    static void LoadSignatureDecoder();
    static void DecodeSignature(std::string &sig) {
        static bool loaded = false;
        if (!loaded) {
            LoadSignatureDecoder();
            loaded = true;
        }

        for (auto &x : SigDecoder) {
            x.first(sig, x.second);
        }
    }
    static void LoadUserPlaylist(Config::Playlist &);
    static void AddToPlaylist(Config::Playlist &, const std::wstring &trackId);
    static void RemoveFromPlaylist(Config::Playlist &, const std::wstring &trackId);

    static void LoadFromUrl(std::wstring url, IAIMPPlaylist *playlist, std::shared_ptr<LoadingState> state, std::function<void()> finishCallback = std::function<void()>());
    static void ResolveUrl(const std::wstring &url, const std::wstring &playlistTitle = std::wstring(), bool createPlaylist = true);

    static void GetExistingTrackIds(IAIMPPlaylist *pl, std::shared_ptr<LoadingState> state);

private:
    static void AddFromJson(IAIMPPlaylist *, const rapidjson::Value &, std::shared_ptr<LoadingState> state);

    YouTubeAPI();
    YouTubeAPI(const YouTubeAPI &);
    YouTubeAPI &operator=(const YouTubeAPI &);

    static DecoderMap SigDecoder;
};
