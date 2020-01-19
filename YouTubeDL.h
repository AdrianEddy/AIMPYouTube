#pragma once

#include <windows.h>
#include <string>

class YouTubeDL {
public:
    static std::wstring GetPath();
    static std::wstring GetStreamUrl(const std::wstring &id);
    static bool Update();

    static std::wstring Params;
    static int Timeout;
    static bool Force;
};
