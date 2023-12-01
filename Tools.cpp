#include "Tools.h"
#include "AIMPYouTube.h"

#include <windows.h>
#include <locale>
#include <sstream>
#include <iomanip>
#include <codecvt>
#include <cctype>
#include <string>
#include <algorithm>
#include <process.h>

#include "SDK/apiGUI.h"
#include "AIMPString.h"

static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> wStrConverter;

std::wstring Tools::ToWString(const std::string &string) {
    return wStrConverter.from_bytes(string);
}

std::wstring Tools::ToWString(const char *string) {
    return wStrConverter.from_bytes(string);
}

std::wstring Tools::ToWString(const rapidjson::Value &val) {
    if (val.IsString() && val.GetStringLength() > 0) {
        const char *ptr = val.GetString();
        return wStrConverter.from_bytes(ptr, ptr + val.GetStringLength());
    }
    return std::wstring();
}

std::string Tools::ToString(const std::wstring &string) {
    return wStrConverter.to_bytes(string);
}

void Tools::OutputLastError(std::wstring *out) {
    DWORD errorMessageID = ::GetLastError();
    if (errorMessageID == 0)
        return;

    LPWSTR messageBuffer = nullptr;
    size_t size = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&messageBuffer, 0, NULL);

    DebugW(L"0x%x: %s", errorMessageID, messageBuffer);
    if (out)
        *out = messageBuffer;

    LocalFree(messageBuffer);
}

void Tools::ShowLastError(std::wstring message) {
    std::wstring error;
    OutputLastError(&error);
    if (!error.empty())
        message += L" - " + error;

    Plugin::instance()->ExecuteInMainThread([message] {
        IAIMPUIMessageDialog* dialog = nullptr;
        IAIMPString* caption = nullptr;
        if (SUCCEEDED(Plugin::instance()->core()->QueryInterface(IID_IAIMPUIMessageDialog, reinterpret_cast<void**>(&dialog))) &&
            SUCCEEDED(Plugin::instance()->LangAIMP(&caption, L"YouTube.Messages\\Error"))) {
            HRESULT r = dialog->Execute(Plugin::instance()->GetMainWindowHandle(), caption, AIMPString(message), MB_OK | MB_ICONERROR);
            caption->Release();
        }
    });
}

std::wstring Tools::UrlEncode(const std::wstring &url) {
    std::wostringstream escaped;
    escaped.fill('0');
    escaped << std::hex;

    for (std::wstring::const_iterator i = url.begin(), n = url.end(); i != n; ++i) {
        const std::wstring::value_type c = (*i);
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            escaped << c;
            continue;
        }
        escaped << '%' << std::setw(2) << int((unsigned char)c);
    }

    return escaped.str();
}

char letter_to_hex(char ch) {
    return isdigit(ch) ? ch - '0' : tolower(ch) - 'a' + 10;
}
std::string Tools::UrlDecode(const std::string &input) {
    std::string decoded;
    auto it = input.begin();
    auto out = std::back_inserter(decoded);
    while (it != input.end()) {
        if (*it == '%') {
            ++it;
            auto v0 = letter_to_hex(*it);
            ++it;
            auto v1 = letter_to_hex(*it);
            ++it;
            *out++ = 0x10 * v0 + v1;
        } else if (*it == '+') {
            *out++ = ' ';
            ++it;
        } else {
            *out++ = *it++;
        }
    }
    return decoded;
}
std::wstring Tools::TrackIdFromUrl(const std::wstring &url) {
    std::wstring id;
    std::wstring::size_type pos, pos_end;
    if (url.find(L"youtube.com") != std::wstring::npos) {
        if ((pos = url.find(L"?v=")) != std::wstring::npos) {
            id = url.c_str() + pos + 3;
        } else if ((pos = url.find(L"&v=")) != std::wstring::npos) {
            id = url.c_str() + pos + 3;
        } else {
            return id;
        }
        if ((pos = id.find(L'&')) != std::wstring::npos)
            id.resize(pos);

        return id;
    } else if (url.find(L"youtu.be") != std::wstring::npos) {
        if ((pos = url.find(L"/", 8)) != std::wstring::npos) {
            id = url.c_str() + pos + 1;
            if ((pos = id.find(L'?')) != std::wstring::npos)
                id.resize(pos);

            if ((pos = id.find(L'&')) != std::wstring::npos)
                id.resize(pos);

            return id;
        }
    } else if (url.find(L"googleapis.com/youtube/v3") != std::wstring::npos) {
        if ((pos = url.find(L"&id=")) != std::wstring::npos) {
            id = url.c_str() + pos + 4;
        } else if ((pos = url.find(L"?id=")) != std::wstring::npos) {
            id = url.c_str() + pos + 4;
        } else {
            return id;
        }
        if ((pos = id.find(L'&')) != std::wstring::npos)
            id.resize(pos);
    } else if ((pos = url.find(L"youtube://")) != std::wstring::npos) {
        pos += 10;
        if ((pos_end = url.find(L"/", pos)) != std::wstring::npos) {
            return url.substr(pos, pos_end - pos);
        } else {
            return url.substr(pos);
        }
    }
    return id;
}

std::string Tools::FindBetween(const std::string &s, const char *from, const char *to, int offset) {
    std::size_t pos, pos2;
    std::size_t fl = strlen(from);
    if ((pos = s.find(from, offset)) != std::string::npos) {
        if (to && (pos2 = s.find(to, pos + fl)) != std::string::npos) {
            return s.substr(pos + fl, pos2 - pos - fl);
        } else {
            return s.substr(pos + fl);
        }
    }
    return std::string();
}

void Tools::ReplaceString(const std::string &search, const std::string &replace, std::string &subject) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
        subject.replace(pos, search.length(), replace);
        pos += replace.length();
    }
}

void Tools::SplitString(const std::string &s, const std::string &delimiter, std::function<void(const std::string &token)> callback) {
    auto start = 0U;
    auto end = s.find(delimiter);
    while (end != std::string::npos) {
        callback(s.substr(start, end - start));
        start = end + delimiter.length();
        end = s.find(delimiter, start);
    }

    callback(s.substr(start, end));
}

std::string Tools::Trim(const std::string &s) {
    auto wsfront = std::find_if_not(s.begin() , s.end(),  [](int c) { return std::isspace(c); });
    auto wsback  = std::find_if_not(s.rbegin(), s.rend(), [](int c) { return std::isspace(c); }).base();
    return (wsback <= wsfront ? std::string() : std::string(wsfront, wsback));
}

Config::TrackInfo *Tools::TrackInfo(const std::wstring &id) {
    if (!id.empty()) {
        if (Config::TrackInfos.find(id) == Config::TrackInfos.end()) {
            if (!Config::ResolveTrackInfo(id))
                return nullptr;
        }
        return &Config::TrackInfos[id];
    }
    return nullptr;
}

Config::TrackInfo *Tools::TrackInfo(IAIMPString *FileName) {
    return TrackInfo(Tools::TrackIdFromUrl(FileName->GetData()));
}

void Tools::ExecuteInNewThread(std::function<void()> &&cb) {
    // If usage of this function becomes common enough, change this to use single thread that we push callbacks to, instead of creating new thread each time
    auto ptr = new std::function<void()>(std::move(cb));
    _beginthread([](void *arg) {
        auto ptr = static_cast<std::function<void()> *>(arg);
        (*ptr)();
        delete ptr;
    }, 0, ptr);
}

