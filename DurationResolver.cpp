#include "DurationResolver.h"
#include "Tools.h"
#include "AimpHTTP.h"
#include "AIMPYouTube.h"
#include <regex>
#include <cmath>
#include <unordered_map>
#include <memory>
#include <vector>

std::vector<DurationResolver::Item> DurationResolver::m_items;

void DurationResolver::AddPlaylist(IAIMPPlaylist *pl) {
    for (int i = 0, n = pl->GetItemCount(); i < n; ++i) {
        IAIMPPlaylistItem *item = nullptr;
        if (SUCCEEDED(pl->GetItem(i, IID_IAIMPPlaylistItem, reinterpret_cast<void **>(&item)))) {
            IAIMPFileInfo *finfo = nullptr;
            if (SUCCEEDED(item->GetValueAsObject(AIMP_PLAYLISTITEM_PROPID_FILEINFO, IID_IAIMPFileInfo, reinterpret_cast<void **>(&finfo)))) {
                IAIMPString *custom = nullptr;
                if (SUCCEEDED(finfo->GetValueAsObject(AIMP_FILEINFO_PROPID_FILENAME, IID_IAIMPString, reinterpret_cast<void **>(&custom)))) {
                    if (auto ti = Tools::TrackInfo(custom)) {
                        if (ti->Duration <= 0) {
                            Item itm;
                            itm.FileInfo = finfo;
                            itm.Id = ti->Id;
                            m_items.push_back(itm);
                        } else {
                            finfo->Release();
                        }
                    }
                    custom->Release();
                }
                // finfo->Release();
            }
            item->Release();
        }
    }
}

void DurationResolver::Resolve() {
    std::wstring allIds;
    auto map = std::make_shared<std::unordered_map<std::wstring, IAIMPFileInfo *>>();
    int n = (std::min)(std::size_t(50), m_items.size());
    for (int i = 0; i < n; ++i) {
        allIds += m_items[i].Id + L",";
        (*map)[m_items[i].Id] = m_items[i].FileInfo;
    }
    m_items.erase(m_items.begin(), m_items.begin() + n);

    if (allIds.size() > 1) {
        allIds.resize(allIds.size() - 1); // Remove trailing comma

        Plugin* plugin = Plugin::instance();
        std::wstring reqUrl(L"https://www.googleapis.com/youtube/v3/videos?part=contentDetails&hl=" + plugin->Lang(L"YouTube\\YouTubeLang") + L"&id=" + allIds);
        reqUrl += L"&key=" + Config::GetString(L"YouTubeKey", TEXT(APP_KEY));
        if (plugin->isConnected() && plugin->useAccount())
            reqUrl += L"\r\nAuthorization: Bearer " + plugin->getAccessToken();

        AimpHTTP::Get(reqUrl, [map](unsigned char *data, int size) {
            rapidjson::Document d;
            d.Parse(reinterpret_cast<const char *>(data));

            if (d.IsObject() && d.HasMember("items")) {
                rapidjson::Value &a = d["items"];
                if (a.IsArray()) {
                    for (auto x = a.Begin(), e = a.End(); x != e; x++) {
                        const rapidjson::Value *px = &(*x);
                        if (!px->IsObject() || !px->HasMember("contentDetails"))
                            continue;

                        const rapidjson::Value &contentDetails = (*px)["contentDetails"];

                        int videoDuration = 0;
                        if (contentDetails.IsObject() && contentDetails.HasMember("duration")) {
                            std::wstring duration = Tools::ToWString(contentDetails["duration"]);

                            std::wregex re(L"PT(?:([0-9]+)H)?(?:([0-9]+)M)?(?:([0-9]+)S)?");
                            std::wsmatch match;
                            if (std::regex_match(duration, match, re)) {
                                auto h = match[1].matched ? std::stoll(match[1].str()) : 0;
                                auto m = match[2].matched ? std::stoll(match[2].str()) : 0;
                                auto s = match[3].matched ? std::stoll(match[3].str()) : 0;
                                videoDuration = h * 3600 + m * 60 + s;

                                std::wstring id = Tools::ToWString((*px)["id"].GetString());
                                if (auto finfo = (*map)[id]) {
                                    finfo->SetValueAsFloat(AIMP_FILEINFO_PROPID_DURATION, videoDuration);

                                    // It was not released previously on purpose, release it now
                                    finfo->Release();
                                    (*map)[id] = nullptr;
                                }

                                if (auto ti = Tools::TrackInfo(id)) {
                                    ti->Duration = videoDuration;
                                }
                            }
                        }
                    }
                }
            }

            for (const auto &x : (*map)) {
                if (x.second)
                    x.second->Release();
            }

            Config::SaveCache();

            if (m_items.size() > 0) {
                Resolve();
            }
        });
    }
}
