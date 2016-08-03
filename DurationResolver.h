#pragma once

#include <vector>
#include "SDK/apiPlaylists.h"
#include "SDK/apiFileManager.h"

class DurationResolver {
public:
    
    static void AddPlaylist(IAIMPPlaylist *pl);

    static void Resolve();

    struct Item {
        IAIMPFileInfo *FileInfo;
        std::wstring Id;
    };
    static std::vector<Item> m_items;
};

