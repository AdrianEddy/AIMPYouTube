#pragma once

#include <windows.h>
#include <string>

class YouTubeDL {
public:
    static std::wstring GetPath();
    static std::wstring GetStreamUrl(const std::wstring &id);
    static bool Update();
};
