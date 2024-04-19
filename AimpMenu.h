#pragma once

#include "SDK/apiCore.h"
#include "SDK/apiMenu.h"
#include "IUnknownInterfaceImpl.h"
#include <functional>
#include <string>

class AimpMenu {
    typedef std::function<void(IAIMPMenuItem *)> CallbackFunc;

    class ClickHandler : public IUnknownInterfaceImpl<IAIMPActionEvent> {
    public:
        ClickHandler(CallbackFunc callback, IAIMPMenuItem *item) : m_menuItem(item), m_callback(callback) {}

        virtual HRESULT WINAPI QueryInterface(REFIID riid, LPVOID* ppvObj) {
            if (!ppvObj) return E_POINTER;

            if (riid == IID_IAIMPActionEvent) {
                *ppvObj = this;
                AddRef();
                return S_OK;
            }

            return E_NOINTERFACE;
        }

        virtual void WINAPI OnExecute(IUnknown *Data) { if (m_callback) m_callback(m_menuItem); }

    private:
        IAIMPMenuItem *m_menuItem;
        CallbackFunc m_callback;
    };

public:
    AimpMenu(IAIMPMenuItem *item);
    ~AimpMenu();

    static bool Init(IAIMPCore *Core);
    static void Deinit();

    IAIMPMenuItem *Add(const std::wstring &name, CallbackFunc action, UINT icon = 0, CallbackFunc showAction = nullptr, const std::wstring &id = std::wstring());
    void Clear();

    static AimpMenu *Get(int id);
    static AimpMenu *Get(const std::wstring &id);
    static bool SetIcon(IAIMPMenuItem *item, UINT icon);

private:
    AimpMenu(const AimpMenu &);
    AimpMenu &operator=(const AimpMenu &);

    IAIMPMenuItem *m_menuItem;

    static IAIMPCore *m_core;
    static IAIMPServiceMenuManager *m_menuManager;
};
