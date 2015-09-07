#pragma once

#include "SDK/apiPlaylists.h"
#include "IUnknownInterfaceImpl.h"

class PlaylistListener : public IUnknownInterfaceImpl<IAIMPExtensionPlaylistManagerListener> {
public:
    virtual HRESULT WINAPI QueryInterface(REFIID riid, LPVOID* ppvObj) {
        if (!ppvObj) return E_POINTER;

        if (riid == IID_IAIMPExtensionPlaylistManagerListener) {
            *ppvObj = this;
            AddRef();
            return S_OK;
        }

        return E_NOINTERFACE;
    }

    virtual void WINAPI PlaylistActivated(IAIMPPlaylist *Playlist);
    virtual void WINAPI PlaylistAdded(IAIMPPlaylist *Playlist);
    virtual void WINAPI PlaylistRemoved(IAIMPPlaylist *Playlist);
};
