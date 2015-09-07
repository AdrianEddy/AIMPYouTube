#pragma once

#include <string>
#include <cinttypes>
#include <functional>
#include "rapidjson/document.h"

#define DebugA(...) { char msg[2048]; sprintf_s(msg, __VA_ARGS__); OutputDebugStringA(msg); }
#define DebugW(...) { wchar_t msg[2048]; wsprintf(msg, __VA_ARGS__); OutputDebugStringW(msg); }

struct Tools {
    static std::wstring ToWString(const std::string &);
    static std::string ToString(const std::wstring &);
    static std::wstring ToWString(const char *);
    static std::wstring ToWString(const rapidjson::Value &);

    static void ReplaceString(const std::string &search, const std::string &replace, std::string &subject);
    static void SplitString(const std::string &string, const std::string &delimiter, std::function<void(const std::string &token)> callback);

    static std::wstring TrackIdFromUrl(const std::wstring &);

    static std::wstring UrlEncode(const std::wstring &);
    static std::string UrlDecode(const std::string &input);
    static void OutputLastError();
};

