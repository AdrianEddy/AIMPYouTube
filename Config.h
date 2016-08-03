#pragma once

#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include "SDK/apiCore.h"
#include <cstdint>
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/filewritestream.h"

#define APP_KEY          "AIzaSyCeEdBFVx7MrZmbEewZCQrCGyJf02UNY8c"
#define CLIENT_ID        "6695011871-r9bnkpp9rkamnkrgim2iniqq38koff09.apps.googleusercontent.com"
#define CLIENT_SECRET    "za2mleqMh2GJCC4ojs4WVYY_"

class Config {
public:
    struct MonitorUrl {
        std::wstring URL;
        std::wstring PlaylistID;
        int Flags;
        std::wstring GroupName;

        typedef rapidjson::PrettyWriter<rapidjson::FileWriteStream, rapidjson::UTF16<>> Writer;
        typedef rapidjson::GenericValue<rapidjson::UTF16<>> Value;

        MonitorUrl(const std::wstring &url, const std::wstring &playlistID, int flags, const std::wstring &groupName = std::wstring())
            : URL(url), PlaylistID(playlistID), Flags(flags), GroupName(groupName) {

        }

        MonitorUrl(const Value &v) {
            if (v.IsObject()) {
                URL = v[L"URL"].GetString();
                Flags = v[L"Flags"].GetInt();
                PlaylistID = v[L"PlaylistID"].GetString();
                GroupName = v[L"GroupName"].GetString();
            }
        }

        friend Writer &operator <<(Writer &writer, const MonitorUrl &that) {
            writer.StartObject();

            writer.String(L"URL");
            writer.String(that.URL.c_str(), that.URL.size());

            writer.String(L"Flags");
            writer.Int(that.Flags);

            writer.String(L"PlaylistID");
            writer.String(that.PlaylistID.c_str(), that.PlaylistID.size());

            writer.String(L"GroupName");
            writer.String(that.GroupName.c_str(), that.GroupName.size());

            writer.EndObject();
            return writer;
        }
    };
    struct Playlist {
        std::wstring ID;
        std::wstring Title;
        std::wstring ChannelName;
        std::wstring ReferenceName;
        std::unordered_set<std::wstring> Items;
        bool CanModify;
        std::wstring AIMPPlaylistId;

        typedef rapidjson::PrettyWriter<rapidjson::FileWriteStream, rapidjson::UTF16<>> Writer;
        typedef rapidjson::GenericValue<rapidjson::UTF16<>> Value;

        Playlist(const std::wstring &id, const std::wstring &title, bool canModify, const std::wstring &refName = std::wstring(), const std::wstring &channelName = std::wstring())
            : ID(id), Title(title), CanModify(canModify), ReferenceName(refName), ChannelName(channelName) {

        }

        Playlist(const Value &v) {
            if (v.IsObject()) {
                ID = v[L"ID"].GetString();
                Title = v[L"Title"].GetString();
                ChannelName = v[L"ChannelName"].GetString();
                ReferenceName = v[L"ReferenceName"].GetString();
                AIMPPlaylistId = v[L"AIMPPlaylistId"].GetString();
                CanModify = v[L"CanModify"].GetBool();
                Items.clear();
                for (auto x = v[L"Items"].Begin(), e = v[L"Items"].End(); x != e; x++) {
                    Items.insert((*x).GetString());
                }
            }
        }

        friend Writer &operator <<(Writer &writer, const Playlist &that) {
            writer.StartObject();

            writer.String(L"ID");
            writer.String(that.ID.c_str(), that.ID.size());

            writer.String(L"Title");
            writer.String(that.Title.c_str(), that.Title.size());

            writer.String(L"ChannelName");
            writer.String(that.ChannelName.c_str(), that.ChannelName.size());

            writer.String(L"ReferenceName");
            writer.String(that.ReferenceName.c_str(), that.ReferenceName.size());

            writer.String(L"AIMPPlaylistId");
            writer.String(that.AIMPPlaylistId.c_str(), that.AIMPPlaylistId.size());

            writer.String(L"CanModify");
            writer.Bool(that.CanModify);

            writer.String(L"Items");
            writer.StartArray();
            for (auto &x : that.Items) {
                writer.String(x.c_str(), x.size());
            }
            writer.EndArray();

            writer.EndObject();
            return writer;
        }
    };
    struct TrackInfo {
        std::wstring Id;
        std::wstring Name;
        std::wstring Permalink;
        std::wstring Artwork;
        double Duration;

        typedef rapidjson::Writer<rapidjson::FileWriteStream, rapidjson::UTF16<>> Writer;
        typedef rapidjson::GenericValue<rapidjson::UTF16<>> Value;

        TrackInfo() : Duration(0) {}
        TrackInfo(const std::wstring &name, const std::wstring &id, const std::wstring &permalink, const std::wstring &artwork, double duration)
            : Name(name), Id(id), Permalink(permalink), Artwork(artwork), Duration(duration) {

        }

        TrackInfo(const Value &v) {
            if (v.IsObject()) {
                Name      = v[L"N"].GetString();
                Permalink = v[L"P"].GetString();
                Artwork   = v[L"A"].GetString();
                Duration  = v[L"D"].GetDouble();
            }
        }

        friend Writer &operator <<(Writer &writer, const TrackInfo &that) {
            writer.StartObject();

            writer.String(L"N");
            writer.String(that.Name.c_str(), that.Name.size());

            writer.String(L"P");
            writer.String(that.Permalink.c_str(), that.Permalink.size());

            writer.String(L"A");
            writer.String(that.Artwork.c_str(), that.Artwork.size());

            writer.String(L"D");
            writer.Double(that.Duration);

            writer.EndObject();
            return writer;
        }
    };

    static bool Init(IAIMPCore *core);
    static void Deinit();

    static void Delete(const std::wstring &name);

    static void SetString(const std::wstring &name, const std::wstring &value);
    static void SetInt64(const std::wstring &name, const int64_t &value);
    static void SetInt32(const std::wstring &name, const int32_t &value);

    static std::wstring GetString(const std::wstring &name, const std::wstring &def = std::wstring());
    static int64_t GetInt64(const std::wstring &name, const int64_t &def = 0);
    static int32_t GetInt32(const std::wstring &name, const int32_t &def = 0);

    static inline std::wstring PluginConfigFolder() { return m_configFolder; }

    static void SaveExtendedConfig();
    static void LoadExtendedConfig();

    static void SaveCache();
    static void LoadCache();
    static bool ResolveTrackInfo(const std::wstring &id);

    static std::unordered_set<std::wstring> TrackExclusions;
    static std::vector<MonitorUrl> MonitorUrls;
    static std::vector<Playlist> UserPlaylists;
    static std::unordered_map<std::wstring, TrackInfo> TrackInfos;

private:
    Config();
    Config(const Config&);
    Config& operator=(const Config&);

    static std::wstring m_configFolder;
    static IAIMPConfig *m_config;
};
