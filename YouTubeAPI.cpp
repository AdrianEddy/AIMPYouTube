#include "YouTubeAPI.h"

#include "AIMPYouTube.h"
#include "AimpHTTP.h"
#include "SDK/apiFileManager.h"
#include "SDK/apiPlaylists.h"
#include "AIMPString.h"
#include "DurationResolver.h"
#include "Tools.h"
#include "Timer.h"
#include "YouTubeDL.h"
#include <Strsafe.h>
#include <string>
#include <set>
#include <map>
#include <regex>

extern DWORD g_MainThreadId;
YouTubeAPI::DecoderMap YouTubeAPI::SigDecoder;
bool YouTubeAPI::ForceYouTubeDL = false;

void YouTubeAPI::AddFromJson(IAIMPPlaylist *playlist, const rapidjson::Value &d, std::shared_ptr<LoadingState> state) {
    if (!playlist || !state || !Plugin::instance()->core())
        return;

    int insertAt = state->InsertPos;
    if (insertAt >= 0)
        insertAt += state->AdditionalPos;

    IAIMPFileInfo *file_info = nullptr;
    if (Plugin::instance()->core()->CreateObject(IID_IAIMPFileInfo, reinterpret_cast<void **>(&file_info)) == S_OK) {
        auto processItem = [&](const std::wstring &pid, const rapidjson::Value &item, const rapidjson::Value &contentDetails) {
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

            auto permalink = L"https://www.youtube.com/watch?v=" + trackId;

            std::wstring filename(L"youtube://");
            filename += trackId + L"/";
            filename += final_title;
            filename += L".mp4";
            file_info->SetValueAsObject(AIMP_FILEINFO_PROPID_FILENAME, AIMPString(filename));

            if (!state->ReferenceName.empty()) {
                file_info->SetValueAsObject(AIMP_FILEINFO_PROPID_ALBUM, AIMPString(state->ReferenceName));
            }
            if (item.HasMember("channelTitle") && (state->Flags & LoadingState::AddChannelTitle)) {
                file_info->SetValueAsObject(AIMP_FILEINFO_PROPID_ARTIST, AIMPString(item["channelTitle"]));
            }
            file_info->SetValueAsObject(AIMP_FILEINFO_PROPID_URL, AIMPString(permalink));

            int64_t videoDuration = 0;
            if (contentDetails.IsObject() && contentDetails.HasMember("duration")) {
                std::wstring duration = Tools::ToWString(contentDetails["duration"]);

                std::wregex re(L"PT(?:([0-9]+)H)?(?:([0-9]+)M)?(?:([0-9]+)S)?");
                std::wsmatch match;
                if (std::regex_match(duration, match, re)) {
                    auto h = match[1].matched ? std::stoll(match[1].str()) : 0;
                    auto m = match[2].matched ? std::stoll(match[2].str()) : 0;
                    auto s = match[3].matched ? std::stoll(match[3].str()) : 0;
                    videoDuration = h * 3600 + m * 60 + s;

                    file_info->SetValueAsFloat(AIMP_FILEINFO_PROPID_DURATION, videoDuration);
                }
            }

            AIMPString title(final_title);
            file_info->SetValueAsObject(AIMP_FILEINFO_PROPID_TITLE, title);

            std::wstring artwork;
            if (item.HasMember("thumbnails") && item["thumbnails"].IsObject() && item["thumbnails"].HasMember("high") && item["thumbnails"]["high"].HasMember("url")) {
                artwork = Tools::ToWString(item["thumbnails"]["high"]["url"]);
            }


            Config::TrackInfos[trackId] = Config::TrackInfo(final_title, trackId, permalink, artwork, videoDuration);

            const DWORD flags = AIMP_PLAYLIST_ADD_FLAGS_FILEINFO | AIMP_PLAYLIST_ADD_FLAGS_NOCHECKFORMAT | AIMP_PLAYLIST_ADD_FLAGS_NOEXPAND | AIMP_PLAYLIST_ADD_FLAGS_NOTHREADING;
            if (SUCCEEDED(playlist->Add(file_info, flags, insertAt))) {
                state->AddedItems++;
                if (insertAt >= 0) {
                    insertAt++;
                    state->InsertPos++;
                    if (state->Flags & LoadingState::UpdateAdditionalPos)
                        state->AdditionalPos++;
                }
            }
        };

        rapidjson::Value null;

        if (d.IsArray()) {
            for (auto x = d.Begin(), e = d.End(); x != e; x++) {
                const rapidjson::Value *px = &(*x);
                if (!px->IsObject() || !px->HasMember("snippet"))
                    continue;

                processItem((*px).HasMember("id") ? Tools::ToWString((*px)["id"]) : L"", (*px)["snippet"], px->HasMember("contentDetails")? (*px)["contentDetails"] : null);
            }
        } else if (d.IsObject() && d.HasMember("snippet")) {
            processItem(d.HasMember("id")? Tools::ToWString(d["id"]) : L"", d["snippet"], d.HasMember("contentDetails") ? d["contentDetails"] : null);
        } else if (d.IsObject()) {
            processItem(L"", d, null);
        }
        file_info->Release();
    }
}

void YouTubeAPI::LoadFromUrl(std::wstring url, IAIMPPlaylist *playlist, std::shared_ptr<LoadingState> state, std::function<void()> finishCallback) {
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
        if (d.IsObject() && d.HasMember("items") && d["items"].IsArray() && d["items"].Size() > 0 && d["items"][0].HasMember("contentDetails") && d["items"][0]["contentDetails"].HasMember("relatedPlaylists")) {
            const rapidjson::Value &i = d["items"][0]["contentDetails"]["relatedPlaylists"];
            std::wstring uploads = Tools::ToWString(i["uploads"]);
            /*std::wstring favorites = Tools::ToWString(i["favorites"]);
            std::wstring likes = Tools::ToWString(i["likes"]);
            std::wstring watchLater = Tools::ToWString(i["watchLater"]);*/
            std::wstring userName = Tools::ToWString(d["items"][0]["snippet"]["localized"]["title"]);
            IAIMPPropertyList *plProp = nullptr;
            if (SUCCEEDED(playlist->QueryInterface(IID_IAIMPPropertyList, reinterpret_cast<void **>(&plProp)))) {
                bool isRenamed = true;
                IAIMPString *str = nullptr;
                if (SUCCEEDED(plProp->GetValueAsObject(AIMP_PLAYLIST_PROPID_NAME, IID_IAIMPString, reinterpret_cast<void **>(&str)))) {
                    isRenamed = wcscmp(L"YouTube", str->GetData()) != 0;
                    str->Release();
                }
                if (!isRenamed)
                    plProp->SetValueAsObject(AIMP_PLAYLIST_PROPID_NAME, AIMPString(userName));
                plProp->Release();
            }
            state->ReferenceName = userName;

            LoadFromUrl(L"https://content.googleapis.com/youtube/v3/playlistItems?part=contentDetails%2Csnippet&maxResults=50&playlistId=" + uploads +
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
            Config::SaveExtendedConfig();

            DurationResolver::AddPlaylist(playlist);
            DurationResolver::Resolve();

            playlist->Release();
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

    auto state = std::make_shared<LoadingState>();
    state->PlaylistToUpdate = &playlist;
    state->ReferenceName = groupName;
    GetExistingTrackIds(pl, state);

    std::wstring url(L"https://content.googleapis.com/youtube/v3/playlistItems?part=contentDetails%2Csnippet&maxResults=50&playlistId=" + playlistId +
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

void YouTubeAPI::GetExistingTrackIds(IAIMPPlaylist *pl, std::shared_ptr<LoadingState> state) {
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
        auto state = std::make_shared<LoadingState>();
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

            finalUrl = L"https://content.googleapis.com/youtube/v3/playlistItems?part=contentDetails%2Csnippet&maxResults=50&playlistId=" + id +
                       L"&fields=items%2Fsnippet%2Ckind%2CnextPageToken%2CpageInfo%2CtokenPagination";
            plName = L"YouTube";
            ytPlaylistId = id;
        } else if (url.find(L"watch?") != std::wstring::npos) {
            std::wstring id = Tools::TrackIdFromUrl(url);
            finalUrl = L"https://www.googleapis.com/youtube/v3/videos?part=contentDetails%2Csnippet&hl=" + Plugin::instance()->Lang(L"YouTube\\YouTubeLang") + L"&id=" + id;
            plName = L"YouTube";
        } else if (url.find(L"youtu.be") != std::wstring::npos) {
            std::wstring id = Tools::TrackIdFromUrl(url);
            finalUrl = L"https://www.googleapis.com/youtube/v3/videos?part=contentDetails%2Csnippet&hl=" + Plugin::instance()->Lang(L"YouTube\\YouTubeLang") + L"&id=" + id;
            plName = L"YouTube";
        }

        IAIMPPlaylist *pl = nullptr;
        std::wstring finalPlaylistName;
        if (createPlaylist) {
            finalPlaylistName = playlistTitle.empty() ? plName : playlistTitle;
            pl = Plugin::instance()->GetPlaylist(finalPlaylistName);
            if (!pl)
                return;
        } else {
            pl = Plugin::instance()->GetCurrentPlaylist();
            if (!pl)
                return;
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
    if (g_MainThreadId == GetCurrentThreadId())
        MessageBox(Plugin::instance()->GetMainWindowHandle(), Plugin::instance()->Lang(L"YouTube.Messages\\CantResolve").c_str(), Plugin::instance()->Lang(L"YouTube.Messages\\Error").c_str(), MB_OK | MB_ICONERROR);
}

std::wstring YouTubeAPI::GetStreamUrl(const std::wstring &id) {
    // TODO: Add ForceYouTubeDL to config

    std::wstring stream_url;
    if (!ForceYouTubeDL) {
        std::wstring url2(L"https://www.youtube.com/get_video_info?video_id=" + id + L"&el=detailpage&sts=16511");
        AimpHTTP::Get(url2, [&](unsigned char *data, int size) {
            if (char *streams = strstr((char *)data, "player_response=")) {
                streams += 16;
                if (char *end = strchr(streams, '&'))
                    *end = 0;
                std::string json = Tools::UrlDecode(streams);

                rapidjson::Document d;
                d.Parse(json.c_str());

                std::vector<int> streamPriority = {
                    -1, // hls
                    // Audio first
                    140, // m4a 128kbps
                    141, // m4a 256kbps
                    256, // m4a
                    258, // m4a
                    // Video
                    22, // mp4 1280x720  (192kbps)
                    37, // mp4 1920x1080 (192kbps)
                    38, // mp4 4096x3072 (192kbps)
                    59, // mp4 854x480 (128kbps)
                    78, // mp4 854x480 (128kbps)
                    135, // mp4 480p
                    134, // mp4 360p
                    136, // mp4 720p
                    137, // mp4 1080p
                    18,  // mp4 640x360 (96kbps)
                    160, // mp4 144p
                    264, // mp4 1440p
                    266, // mp4 2160p
                    133 // mp4 240p
                };

                std::map<int, std::wstring> urls;

                std::wstring medium_stream;

                if (d.HasMember("streamingData") && d["streamingData"].IsObject()) {
                    rapidjson::Value formats;
                         if (d["streamingData"].HasMember("formats")         && d["streamingData"]["formats"].IsArray()) formats = d["streamingData"]["formats"].GetArray();
                    else if (d["streamingData"].HasMember("adaptiveFormats") && d["streamingData"]["adaptiveFormats"].IsArray()) formats = d["streamingData"]["adaptiveFormats"].GetArray();

                    for (auto x = formats.Begin(), e = formats.End(); x != e; x++) {
                        const rapidjson::Value &px = *x;
                        if (!px.IsObject())
                            continue;

                        std::string stream;
                        if (px.HasMember("url")) stream = px["url"].GetString();
                        if (stream.empty() && px.HasMember("cipher")) {
                            std::string s, sig, sp = "signature";

                            Tools::SplitString(px["cipher"].GetString(), "&", [&](const std::string &token) {
                                     if (token.find("url=")  == 0) { stream = Tools::UrlDecode(token.substr(4)); }
                                else if (token.find("s=")    == 0) { s      = Tools::UrlDecode(token.substr(2)); YouTubeAPI::DecodeSignature(s); }
                                else if (token.find("sp=")   == 0) { sp     = Tools::UrlDecode(token.substr(3)); }
                                else if (token.find("sig=")  == 0) { sig    = Tools::UrlDecode(token.substr(4)); }
                            });

                            if (s.empty() && !sig.empty())
                                s = sig;

                            stream += "&" + sp + "=" + s;
                        }
                        std::wstring wstream = Tools::ToWString(stream);

                        if (px.HasMember("itag") && px["itag"].IsInt()) {
                            int itag = px["itag"].GetInt();
                            urls[itag] = wstream;
                        }

                        if (px.HasMember("audioQuality") && strcmp(px["audioQuality"].GetString(), "AUDIO_QUALITY_MEDIUM") == 0 && px.HasMember("mimeType") && strstr(px["mimeType"].GetString(), "mp4") != nullptr) {
                            medium_stream = wstream;
                        }
                    }

                    if (d["streamingData"].HasMember("hlsManifestUrl") && d["streamingData"]["hlsManifestUrl"].IsString()) {
                        urls[-1] = Tools::ToWString(d["streamingData"]["hlsManifestUrl"].GetString());
                    }
                }
                for (auto x : streamPriority) {
                    if (urls.find(x) != urls.end()) {
                        stream_url = urls[x];
                        break;
                    }
                }

                if (stream_url.empty() && !medium_stream.empty()) {
                    stream_url = medium_stream;
                }

                // If none of preferred streams are available, get the first one
                if (stream_url.empty() && urls.size() > 0) {
                    stream_url = urls.begin()->second;
                }
            }
        }, true);
    }

    if (stream_url.empty() || ForceYouTubeDL) {
        stream_url = YouTubeDL::GetStreamUrl(id);
        static bool updated = false;
        if (stream_url.empty() && !updated) {
            updated = true;
            YouTubeDL::Update();
        }
    }

    return stream_url;
}

void YouTubeAPI::LoadSignatureDecoder() {
    static std::map<std::string, std::function<void(std::string &s, int param)>> mutatorTypes {
        { "swap",    [](std::string &s, int param) { std::swap(s[0], s[param]);        } },
        { "erase",   [](std::string &s, int param) { s.erase(0, param);                } },
        { "reverse", [](std::string &s, int param) { std::reverse(s.begin(), s.end()); } },
    };

    std::wstring ua(L"Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/79.0.3945.117 Safari/537.36");
    AimpHTTP::Get(L"https://www.youtube.com/\r\nUser-Agent: " + ua, [&](unsigned char *data1, int) {
        std::string player = Tools::FindBetween((char *)data1, "\"js\":\"", "\"");
        if (!player.empty()) {
            Tools::ReplaceString("\\/", "/", player);
            if (player.find("http") == std::string::npos)
                player = "https://www.youtube.com" + player;

            player += "\r\nAccept-Encoding: gzip;q=0,deflate;q=0,identity";

            AimpHTTP::Get(Tools::ToWString(player), [&](unsigned char *rawData, int size) {
                std::string data(reinterpret_cast<char *>(rawData), size);
                Tools::ReplaceString("\n", "", data);
                Tools::ReplaceString("\r", "", data);

                // Taken from youtube-dl
                // https://github.com/ytdl-org/youtube-dl/blob/master/youtube_dl/extractor/youtube.py#L1342
                std::vector<std::regex> patterns {
                    std::regex(R"PATTERN(\b[cs]\s*&&\s*[adf]\.set\([^,]+\s*,\s*encodeURIComponent\s*\(\s*([a-zA-Z0-9$]+)\()PATTERN"),
                    std::regex(R"PATTERN(\b[a-zA-Z0-9]+\s*&&\s*[a-zA-Z0-9]+\.set\([^,]+\s*,\s*encodeURIComponent\s*\(\s*([a-zA-Z0-9$]+)\()PATTERN"),
                    std::regex(R"PATTERN(([a-zA-Z0-9$]+)\s*=\s*function\(\s*a\s*\)\s*\{\s*a\s*=\s*a\.split\(\s*""\s*\))PATTERN")
                };
                std::smatch m;
                std::string funcsig;
                for (const auto &x : patterns) {
                    if (std::regex_search(data, m, x)) {
                        funcsig = ";" + m[1].str() + "=function";
                        break;
                    }
                }

                if (!funcsig.empty()) {
                    std::string sstr = Tools::FindBetween(data, funcsig.c_str(), "join(\"\")");
                    if (sstr.size() > 5) {
                        sstr.erase(0, 4);
                        std::string mutatorObjectName = "var " + Tools::FindBetween(sstr, "split(\"\");", ".") + "={";
                        std::string mutStr = Tools::FindBetween(data, mutatorObjectName.c_str(), "};");

                        if (!mutStr.empty()) {
                            std::map<std::string, std::string> mutators;
                            Tools::SplitString(mutStr, "},", [&](std::string token) {
                                token = Tools::Trim(token);
                                std::string name(token.substr(0, 2));
                                if (token.find("var ")    != std::string::npos) mutators[name] = "swap";
                                if (token.find("splice")  != std::string::npos) mutators[name] = "erase";
                                if (token.find("reverse") != std::string::npos) mutators[name] = "reverse";
                            });

                            Tools::SplitString(sstr, ";", [&](std::string token) {
                                token = Tools::Trim(token);
                                if (token.find("split") != std::string::npos || token.find("return") != std::string::npos)
                                    return;

                                std::string mutator(token.substr(3, 2));
                                if (char *params = strchr((char *)token.c_str(), ',')) {
                                    if (char *end = strchr(params, ')')) *end = 0;
                                    int param = std::stoi(params + 1);
                                    if (mutators.find(mutator) != mutators.end()) {
                                        DebugA("\t['%s', %d],\n", mutators[mutator].c_str(), param);
                                        if (mutatorTypes.find(mutators[mutator]) != mutatorTypes.end()) {
                                            YouTubeAPI::SigDecoder.push_back({ mutatorTypes[mutators[mutator]], param });
                                        }
                                    } else {
                                        DebugA("Unknown mutator: %s\n", mutator.c_str());
                                    }
                                }
                            });
                        }
                    }
                }
            }, true);
        }
    }, true);

    if (YouTubeAPI::SigDecoder.empty())
        ForceYouTubeDL = true;
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

