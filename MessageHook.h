#pragma once

#include "SDK/apiMessages.h"
#include "IUnknownInterfaceImpl.h"

class Plugin;

class MessageHook : public IUnknownInterfaceImpl<IAIMPMessageHook> {
public:
    MessageHook(Plugin *pl);
    ~MessageHook();

    virtual HRESULT WINAPI QueryInterface(REFIID riid, LPVOID* ppvObj) {
        if (!ppvObj) return E_POINTER;

        if (riid == IID_IAIMPMessageHook) {
            *ppvObj = this;
            AddRef();
            return S_OK;
        }

        return E_NOINTERFACE;
    }

    virtual void WINAPI CoreMessage(DWORD AMessage, int AParam1, void *AParam2, HRESULT *AResult);

private:
    Plugin *m_plugin;
};
