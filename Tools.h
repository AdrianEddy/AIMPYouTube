#pragma once

#include <string>
#include <cinttypes>
#include <functional>
#include "Config.h"
#include "Strsafe.h"
#include "rapidjson/document.h"

#define DebugA(...) { char msg[2048]; sprintf_s(msg, __VA_ARGS__); OutputDebugStringA(msg); }
#define DebugW(...) { wchar_t msg[2048]; StringCchPrintfW(msg, sizeof(msg)/sizeof(wchar_t), __VA_ARGS__); OutputDebugStringW(msg); }

struct Tools {
    static std::wstring ToWString(const std::string &);
    static std::string ToString(const std::wstring &);
    static std::wstring ToWString(const char *);
    static std::wstring ToWString(const rapidjson::Value &);

    static std::string FindBetween(const std::string &s, const char *from, const char *to = nullptr, int offset = 0);
    static void ReplaceString(const std::string &search, const std::string &replace, std::string &subject);
    static void SplitString(const std::string &string, const std::string &delimiter, std::function<void(const std::string &token)> callback);
    static  std::string Trim(const std::string &s);

    static std::wstring TrackIdFromUrl(const std::wstring &);
    static Config::TrackInfo *TrackInfo(const std::wstring &id);
    static Config::TrackInfo *TrackInfo(IAIMPString *FileName);

    static std::wstring UrlEncode(const std::wstring &);
    static std::string UrlDecode(const std::string &input);
    static void OutputLastError(std::wstring *out = nullptr);
    static void ShowLastError(std::wstring message);

    static void ExecuteInNewThread(std::function<void()> &&cb);

    static bool HideErrors;
};

