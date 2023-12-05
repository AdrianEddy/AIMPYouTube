#pragma once

#include <windows.h>
#include <string>

class YTAPIDialog {
public:
    static void Show();

private:
    YTAPIDialog();
    YTAPIDialog(const YTAPIDialog&);
    YTAPIDialog& operator=(const YTAPIDialog&);
    
    static BOOL CALLBACK DlgProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam);
    static void UpdateHint(HWND hwnd, std::wstring &key, std::wstring &clientID, std::wstring &clientSecret);
};
