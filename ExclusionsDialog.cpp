#include "ExclusionsDialog.h"
#include "resource.h"
#include "Tools.h"
#include "YouTubeAPI.h"
#include "GdiPlusImageLoader.h"
#include "AIMPYouTube.h"
#include "OptionsDialog.h"
#include <Commctrl.h>
#include <unordered_map>
#include <memory>

typedef __declspec(dllimport) HRESULT(__stdcall *SetWindowThemeFuncPtr)(HWND hwnd, LPCWSTR pszSubAppName, LPCWSTR pszSubIdList);
HRESULT SetExplorerTheme(HWND hwnd) {
    static HMODULE DLL = LoadLibrary(L"UxTheme.dll");
    static SetWindowThemeFuncPtr setWindowTheme = nullptr;

    if (DLL) {
        if (!setWindowTheme)
            setWindowTheme = (SetWindowThemeFuncPtr)GetProcAddress(DLL, "SetWindowTheme");

        if (setWindowTheme) 
            return setWindowTheme(hwnd, L"Explorer", NULL);
    }
    return E_FAIL;
}

extern HINSTANCE g_hInst;

#define WM_SUBCLASSINIT WM_USER + 4

void ExclusionsDialog::Show(HWND parent) {
    if (!parent)
        parent = Plugin::instance()->GetMainWindowHandle();

    DialogBox(g_hInst, MAKEINTRESOURCE(IDD_EXCLUSIONS), parent, DlgProc);
}

BOOL CALLBACK ExclusionsDialog::DlgProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
    switch (Msg) {
        case WM_CLOSE:
            EndDialog(hwnd, IDCANCEL);
        break;
        case WM_NOTIFY: {
            auto lv = GetDlgItem(hwnd, IDC_LISTVIEW);
            if (LPNMHDR(lParam)->hwndFrom == lv) {
                switch (LPNMHDR(lParam)->code) {
                    case NM_RCLICK: {
                        int i = ListView_GetNextItem(lv, -1, LVNI_SELECTED);
                        if (i != -1) {
                            POINT cursor;
                            GetCursorPos(&cursor);

                            HMENU popup = CreatePopupMenu();
                            HMENU playlistsMenu = CreatePopupMenu();

                            int p = 0;
                            std::unordered_map<UINT_PTR, IAIMPPlaylist *> plMap;
                            Plugin::instance()->ForAllPlaylists([&](IAIMPPlaylist *pl, const std::wstring &name) {
                                UINT_PTR id = 0x57d100 + p++;
                                AppendMenu(playlistsMenu, MF_STRING, id, name.c_str());
                                plMap[id] = pl;
                            });

                            AppendMenu(popup, MF_STRING, 0x57d001, Plugin::instance()->Lang(L"YouTube.Exclusions\\Remove").c_str());
                            AppendMenu(popup, MF_STRING | MF_POPUP, (UINT_PTR)playlistsMenu, Plugin::instance()->Lang(L"YouTube.Exclusions\\RemoveAndAddToPlaylist").c_str());
                            AppendMenu(popup, MF_SEPARATOR, 0, NULL);
                            AppendMenu(popup, MF_STRING, 0x57d003, Plugin::instance()->Lang(L"YouTube.Menu\\OpenInBrowser").c_str());

                            auto result = TrackPopupMenu(popup, TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD, cursor.x, cursor.y, 0, hwnd, NULL);
                            DestroyMenu(playlistsMenu);
                            DestroyMenu(popup);

                            while (i != -1) {
                                LVITEM selectedItem;
                                selectedItem.mask = LVIF_PARAM;
                                selectedItem.iItem = i;
                                ListView_GetItem(lv, (LVITEM *)&selectedItem);

                                if (auto ti = reinterpret_cast<Config::TrackInfo *>(selectedItem.lParam)) {
                                    switch (result) {
                                        case 0x57d001: // remove from exclusions
                                            Config::TrackExclusions.erase(ti->Id);
                                            ListView_DeleteItem(lv, i--);
                                        break;
                                        case 0x57d003: // open in web browser
                                            if (!ti->Permalink.empty())
                                                ShellExecute(Plugin::instance()->GetMainWindowHandle(), L"open", ti->Permalink.c_str(), NULL, NULL, SW_SHOWNORMAL);
                                        break;
                                        default:
                                            if (auto pl = plMap[result]) {
                                                Config::TrackExclusions.erase(ti->Id);

                                                auto state = std::make_shared<YouTubeAPI::LoadingState>();
                                                std::wstring url = L"https://www.googleapis.com/youtube/v3/videos?part=contentDetails%2Csnippet&hl=" + Plugin::instance()->Lang(L"YouTube\\YouTubeLang") + L"&id=" + ti->Id;
                                                YouTubeAPI::LoadFromUrl(url, pl, state);

                                                ListView_DeleteItem(lv, i--);
                                            }
                                        break;
                                    }
                                }
                                i = ListView_GetNextItem(lv, i, LVNI_SELECTED);
                            }
                            Config::SaveExtendedConfig();
                        }
                    } break;
                } break;
            }
        } break;
        case WM_INITDIALOG: {
            SendDlgItemMessage(hwnd, IDC_CREATENEW, BM_SETCHECK, BST_CHECKED, NULL);
            GdiPlusImageLoader icon(IDB_ICON, L"PNG");
            HICON hiIcon;
            HBITMAP hbIcon;
            icon->GetHICON(&hiIcon);
            icon->GetHBITMAP(NULL, &hbIcon);

            SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hiIcon);
            DeleteObject(hiIcon);

            SetWindowText (hwnd,                         Plugin::instance()->Lang(L"YouTube.Exclusions\\Title").c_str());
            SetDlgItemText(hwnd, IDOK,                   Plugin::instance()->Lang(L"YouTube.Exclusions\\OK").c_str());
            SetDlgItemText(hwnd, IDC_EXCLUSIONSGROUPBOX, Plugin::instance()->Lang(L"YouTube.Exclusions\\Header").c_str());
            
            auto lv = GetDlgItem(hwnd, IDC_LISTVIEW);

            SetWindowSubclass(GetDlgItem(hwnd, IDC_EXCLUSIONSGROUPBOX), OptionsDialog::GroupBoxProc, 0, 0); SendDlgItemMessage(hwnd, IDC_EXCLUSIONSGROUPBOX, WM_SUBCLASSINIT, 0, 0);

            ListView_SetExtendedListViewStyle(lv, LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER);
            SetExplorerTheme(lv);

            LVCOLUMN lvc;
            lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM | LVIF_IMAGE;

            static auto h1 = Plugin::instance()->Lang(L"YouTube.Exclusions\\ListHeader", 0);
            static auto h2 = Plugin::instance()->Lang(L"YouTube.Exclusions\\ListHeader", 1);

            lvc.iSubItem = 0; lvc.cx = 450; lvc.pszText = const_cast<LPWSTR>(h1.c_str()); ListView_InsertColumn(lv, 0, &lvc);
            lvc.iSubItem = 1; lvc.cx = 50; lvc.pszText = const_cast<LPWSTR>(h2.c_str()); ListView_InsertColumn(lv, 1, &lvc);

            HIMAGELIST himl = ImageList_Create(24, 24, ILC_COLOR32, 3, 0);
            ImageList_Add(himl, hbIcon, (HBITMAP)NULL);
            DeleteObject(hbIcon);

            LVITEM lvi;
            lvi.mask = LVIF_TEXT | LVIF_PARAM;

            int i = 0;
            wchar_t buf[16];
            for (auto x : Config::TrackExclusions) {
                if (auto ti = Tools::TrackInfo(x)) {
                    if (ti->Duration >= 0) {
                        lvi.pszText = const_cast<wchar_t *>(ti->Name.c_str());
                        lvi.iItem = i;
                        lvi.iSubItem = 0;
                        lvi.iImage = 0;
                        lvi.lParam = reinterpret_cast<LPARAM>(ti);
                        ListView_InsertItem(lv, &lvi);

                        unsigned int hours = floor(ti->Duration / 3600.0);
                        if (hours > 0) {
                            swprintf_s(buf, L"%d:%02d:%02d", hours, (uint32_t)floor(fmod(ti->Duration, 3600.0) / 60.0), (uint32_t)floor(fmod(ti->Duration, 60.0)));
                        } else {
                            swprintf_s(buf, L"%d:%02d", (uint32_t)floor(fmod(ti->Duration, 3600.0) / 60.0), (uint32_t)floor(fmod(ti->Duration, 60.0)));
                        }

                        ListView_SetItemText(lv, i, 1, buf);
                    }
                }
            }

            ListView_SetImageList(lv, himl, LVSIL_SMALL);

            ListView_SetColumnWidth(lv, 1, LVSCW_AUTOSIZE_USEHEADER);

            SetFocus(lv);
        } break;
        case WM_COMMAND: {
            switch (LOWORD(wParam)) {
                case IDOK: {
                    EndDialog(hwnd, wParam);
                } break;
            }
        } break;
        default: return FALSE;
    }
    return TRUE;
}
