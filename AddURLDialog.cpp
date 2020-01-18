#include "AddURLDialog.h"
#include "resource.h"
#include "Tools.h"
#include "YouTubeAPI.h"
#include "GdiPlusImageLoader.h"
#include "AIMPYouTube.h"

extern HINSTANCE g_hInst;

void AddURLDialog::Show() {
    HWND parent = Plugin::instance()->GetMainWindowHandle();

    DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ADDURL), parent, DlgProc);
}

BOOL CALLBACK AddURLDialog::DlgProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
    switch (Msg) {
        case WM_CLOSE:
            EndDialog(hwnd, IDCANCEL);
        break;
        case WM_INITDIALOG: {
            int createNew = Config::GetInt32(L"CreateNewPlaylist", 1);
            SendDlgItemMessage(hwnd, IDC_CREATENEW, BM_SETCHECK, createNew? BST_CHECKED : BST_UNCHECKED, NULL);
            EnableWindow(GetDlgItem(hwnd, IDC_PLAYLISTTITLE), createNew);
            EnableWindow(GetDlgItem(hwnd, IDC_PLAYLISTTITLECAPTION), createNew);
            GdiPlusImageLoader icon(IDB_ICON, L"PNG");
            HICON bitmap;
            icon->GetHICON(&bitmap);
            SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)bitmap);
            DeleteObject(bitmap);

            SetWindowText (hwnd,                           Plugin::instance()->Lang(L"YouTube.AddURL\\Title").c_str());
            SetDlgItemText(hwnd, IDC_YouTubeURLCAPTION,    Plugin::instance()->Lang(L"YouTube.AddURL\\URL").c_str());
            SetDlgItemText(hwnd, IDC_CREATENEW,            Plugin::instance()->Lang(L"YouTube.AddURL\\CreateNew").c_str());
            SetDlgItemText(hwnd, IDOK,                     Plugin::instance()->Lang(L"YouTube.AddURL\\OK").c_str());
            SetDlgItemText(hwnd, IDC_PLAYLISTTITLECAPTION, Plugin::instance()->Lang(L"YouTube.AddURL\\PlaylistName").c_str());
            SetFocus(GetDlgItem(hwnd, IDC_YouTubeURL));
        } break;
        case WM_COMMAND: {
            switch (LOWORD(wParam)) {
                case IDOK: {
                    wchar_t buf[1024];
                    GetDlgItemText(hwnd, IDC_YouTubeURL, buf, 1024);
                    std::wstring url(buf);

                    GetDlgItemText(hwnd, IDC_PLAYLISTTITLE, buf, 1024);
                    std::wstring playlistTitle(buf);

                    bool createnew = SendDlgItemMessage(hwnd, IDC_CREATENEW, BM_GETCHECK, NULL, NULL) == BST_CHECKED;
                    Config::SetInt32(L"CreateNewPlaylist", createnew);

                    if (!url.empty()) {
                        YouTubeAPI::ResolveUrl(url, playlistTitle, createnew);
                    }
                    EndDialog(hwnd, wParam);
                } break;
                case IDCANCEL:
                    EndDialog(hwnd, wParam);
                break;
                case IDC_CREATENEW:
                    if (HIWORD(wParam) == BN_CLICKED) {
                        LRESULT chkState = SendMessage((HWND)lParam, BM_GETCHECK, 0, 0);
                        BOOL enable = chkState == BST_CHECKED;

                        EnableWindow(GetDlgItem(hwnd, IDC_PLAYLISTTITLE), enable);
                        EnableWindow(GetDlgItem(hwnd, IDC_PLAYLISTTITLECAPTION), enable);
                    }
                break;
            }
        } break;
        default: return FALSE;
    }
    return TRUE;
}
