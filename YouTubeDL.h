#pragma once

#include <string>

class YouTubeDL {
public:
    static std::wstring GetPath();
    static std::wstring GetStreamUrl(const std::wstring &id);
    static bool Update(bool foreground = true);

    static std::wstring Params;
    static int Timeout;
    static bool Force;
};
