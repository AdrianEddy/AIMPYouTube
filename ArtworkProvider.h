#pragma once

#include "IUnknownInterfaceImpl.h"
#include "SDK/apiAlbumArt.h"
#include "Tools.h"

class ArtworkProvider : public IUnknownInterfaceImpl<IAIMPExtensionAlbumArtProvider2> {
    virtual HRESULT WINAPI QueryInterface(REFIID riid, LPVOID *ppvObj) {
        if (!ppvObj) return E_POINTER;

        if (riid == IID_IAIMPExtensionAlbumArtProvider) {
            *ppvObj = static_cast<IAIMPExtensionAlbumArtProvider *>(this);
            AddRef();
            return S_OK;
        }
        if (riid == IID_IAIMPExtensionAlbumArtProvider2) {
            *ppvObj = static_cast<IAIMPExtensionAlbumArtProvider2 *>(this);
            AddRef();
            return S_OK;
        }

        return E_NOINTERFACE;
    }
    virtual DWORD WINAPI GetCategory() { return AIMP_ALBUMART_PROVIDER_CATEGORY_FILE; }

    virtual HRESULT WINAPI Get(IAIMPString *FileURI, IAIMPString *Artist, IAIMPString *Album, IAIMPPropertyList *Options, IAIMPImageContainer **Image) { return E_NOTIMPL; }
    virtual HRESULT WINAPI Get2(IAIMPFileInfo *FileInfo, IAIMPPropertyList *Options, IAIMPImageContainer **Image);
};
