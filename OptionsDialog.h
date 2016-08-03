#pragma once

#include <windows.h>
#include <functional>
#include <cinttypes>
#include <vector>
#include "IUnknownInterfaceImpl.h"
#include "SDK/apiOptions.h"
#include "Config.h"

class Plugin;

class OptionsDialog : public IUnknownInterfaceImpl<IAIMPOptionsDialogFrame>, public IAIMPOptionsDialogFrameKeyboardHelper {
public:
    typedef IUnknownInterfaceImpl<IAIMPOptionsDialogFrame> Base;

    OptionsDialog(Plugin *plugin);
    ~OptionsDialog();

    virtual HRESULT WINAPI QueryInterface(REFIID riid, LPVOID* ppvObj) {
        if (!ppvObj) return E_POINTER;

        if (riid == IID_IAIMPOptionsDialogFrame) {
            *ppvObj = this;
            AddRef();
            return S_OK;
        }
        if (riid == IID_IAIMPOptionsDialogFrameKeyboardHelper) {
            *ppvObj = static_cast<IAIMPOptionsDialogFrameKeyboardHelper *>(this);;
            AddRef();
            return S_OK;
        }

        return E_NOINTERFACE;
    }
    virtual ULONG WINAPI AddRef(void) { return Base::AddRef(); }
    virtual ULONG WINAPI Release(void) { return Base::Release(); }

    HRESULT WINAPI GetName(IAIMPString **S);

    HWND WINAPI CreateFrame(HWND ParentWnd);
    void WINAPI DestroyFrame();

    void WINAPI Notification(int ID);

    void LoadProfileInfo(std::function<void()> onFinished = nullptr);
    void UpdateProfileInfo();

    void OptionsModified();

    virtual BOOL WINAPI DialogChar(WCHAR CharCode, int Unused) { return false; }
    virtual BOOL WINAPI DialogKey(WORD CharCode, int Unused) { return false; }
    virtual BOOL WINAPI SelectFirstControl();
    virtual BOOL WINAPI SelectNextControl(BOOL FindForward, BOOL CheckTabStop);

    static void Connect(std::function<void()>);

private:
    std::wstring m_userId;
    std::wstring m_userName;
    std::wstring m_userYTName;
    std::wstring m_userInfo;
    std::vector<Config::Playlist> m_userPlaylists;

    HWND m_handle;
    unsigned int m_currentFocusControl;
    Plugin *m_plugin;

    IAIMPServiceOptionsDialog *m_dialogService;

    static BOOL CALLBACK DlgProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam);

    static LRESULT CALLBACK ButtonProc  (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
    static LRESULT CALLBACK FrameProc   (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
    static LRESULT CALLBACK GroupBoxProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
    static LRESULT CALLBACK AvatarProc  (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
    static LRESULT CALLBACK LinkProc    (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

    friend class ExclusionsDialog;
};
