#pragma once

#include <windows.h>

class ExclusionsDialog {
public:
    static void Show(HWND parent = nullptr);

private:
    ExclusionsDialog();
    ExclusionsDialog(const ExclusionsDialog &);
    ExclusionsDialog &operator=(const ExclusionsDialog &);

    static BOOL CALLBACK DlgProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK ListViewProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
};
