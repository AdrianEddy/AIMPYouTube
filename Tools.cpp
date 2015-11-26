#include "Tools.h"

#include <windows.h>
#include <locale>
#include <sstream>
#include <iomanip>
#include <codecvt>
#include <cctype>
#include <string>
#include <algorithm>

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

void Tools::OutputLastError() {
    DWORD errorMessageID = ::GetLastError();
    if (errorMessageID == 0)
        return;

    LPWSTR messageBuffer = nullptr;
    size_t size = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&messageBuffer, 0, NULL);

    DebugW(L"0x%x: %s", errorMessageID, messageBuffer);

    LocalFree(messageBuffer);
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
    std::wstring::size_type pos;
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
    } else if (url.find(L"localhost") != std::wstring::npos) {
        if ((pos = url.find(L"/", 8)) != std::wstring::npos) {
            return url.c_str() + pos + 1;
        }
    }
    return id;
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

