#pragma once

#include "IUnknownInterfaceImpl.h"
#include "SDK/apiPlayer.h"

class PlayerHook : public IUnknownInterfaceImpl<IAIMPExtensionPlayerHook> {
    virtual HRESULT WINAPI QueryInterface(REFIID riid, LPVOID* ppvObj) {
        if (!ppvObj) return E_POINTER;

        if (riid == IID_IAIMPExtensionPlayerHook) {
            *ppvObj = this;
            AddRef();
            return S_OK;
        }

        return E_NOINTERFACE;
    }

    virtual HRESULT WINAPI OnCheckURL(IAIMPString *URL, BOOL **Handled);
};
