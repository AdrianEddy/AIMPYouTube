#include "YTAPIDialog.h"
#include "resource.h"
#include "AIMPYouTube.h"

extern HINSTANCE g_hInst;

void YTAPIDialog::Show() {
    HWND parent = Plugin::instance()->GetMainWindowHandle();

    DialogBox(g_hInst, MAKEINTRESOURCE(IDD_YTAPISETTINGS), parent, DlgProc);
}

BOOL CALLBACK YTAPIDialog::DlgProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
    switch (Msg) {
        case WM_CLOSE:
            EndDialog(hwnd, IDCANCEL);
            break;
        case WM_INITDIALOG: {
            dialog = (OptionsDialog*)lParam;
            std::wstring key =          Config::GetString(L"YouTubeKey", L"");
            std::wstring clientID =     Config::GetString(L"YouTubeClientID", L"");
            std::wstring clientSecret = Config::GetString(L"YouTubeClientSecret", L"");

            SendDlgItemMessage(hwnd, IDC_YTAPIKEY,          WM_SETTEXT, 0, (LPARAM)key.c_str());
            SendDlgItemMessage(hwnd, IDC_YTAPICLIENTID,     WM_SETTEXT, 0, (LPARAM)clientID.c_str());
            SendDlgItemMessage(hwnd, IDC_YTAPICLIENTSECRET, WM_SETTEXT, 0, (LPARAM)clientSecret.c_str());

            SetWindowText(hwnd, Plugin::instance()->Lang(L"YouTube.YTAPI\\Title").c_str());
            SetDlgItemText(hwnd, IDC_YTAPIKEYTEXT, Plugin::instance()->Lang(L"YouTube.YTAPI\\Key").c_str());
            SetDlgItemText(hwnd, IDC_YTAPICLIENTIDTEXT, Plugin::instance()->Lang(L"YouTube.YTAPI\\ClientID").c_str());
            SetDlgItemText(hwnd, IDC_YTAPICLIENTSECRETTEXT, Plugin::instance()->Lang(L"YouTube.YTAPI\\ClientSecret").c_str());
            SetDlgItemText(hwnd, IDOK, Plugin::instance()->Lang(L"YouTube.YTAPI\\OK").c_str());
            SetDlgItemText(hwnd, IDCANCEL, Plugin::instance()->Lang(L"YouTube.YTAPI\\Cancel").c_str());
            SetFocus(GetDlgItem(hwnd, IDC_YTAPIKEY));
        } break;
        case WM_COMMAND: {
            switch (LOWORD(wParam)) {
                case IDOK: {
                    wchar_t buf[1024];
                    GetDlgItemText(hwnd, IDC_YTAPIKEY, buf, 1024);
                    std::wstring key(buf);

                    GetDlgItemText(hwnd, IDC_YTAPICLIENTID, buf, 1024);
                    std::wstring clientID(buf);

                    GetDlgItemText(hwnd, IDC_YTAPICLIENTSECRET, buf, 1024);
                    std::wstring clientSecret(buf);

                    std::wstring oldClientID = Config::GetString(L"YouTubeClientID", L"");

                    if (key.empty()) {
                        Config::Delete(L"YouTubeKey");
                    } else {
                        Config::SetString(L"YouTubeKey", key);
                    }

                    if (clientID.empty()) {
                        Config::Delete(L"YouTubeClientID");
                        Config::Delete(L"YouTubeClientSecret");
                    } else {
                        Config::SetString(L"YouTubeClientID",     clientID);
                        Config::SetString(L"YouTubeClientSecret", clientSecret);
                    }

                    EndDialog(hwnd, wParam);
                } break;
                case IDCANCEL:
                    EndDialog(hwnd, wParam);
                break;
            }
            switch (HIWORD(wParam)) {
                case EN_CHANGE: {
                    wchar_t buf[1024];
                    GetDlgItemText(hwnd, IDC_YTAPIKEY, buf, 1024);
                    std::wstring key(buf);

                    GetDlgItemText(hwnd, IDC_YTAPICLIENTID, buf, 1024);
                    std::wstring clientID(buf);

                    GetDlgItemText(hwnd, IDC_YTAPICLIENTSECRET, buf, 1024);
                    std::wstring clientSecret(buf);

                    UpdateHint(hwnd, key, clientID, clientSecret);
                } break;
            }
        } break;
        default: return FALSE;
    }
    return TRUE;
}

void YTAPIDialog::UpdateHint(HWND hwnd, std::wstring &key, std::wstring &clientID, std::wstring &clientSecret) {
    // Only a key or key and client ID and secret are required
    std::wstring langKey;
    bool isValid = true;
    bool keySet = !key.empty();
    bool clientIDSet = !clientID.empty();
    bool clientSecretSet = !clientSecret.empty();
    if (!keySet && !clientIDSet && !clientSecretSet) {
        langKey = L"YouTube.YTAPI\\HintNone";
    } else if ((clientIDSet || clientSecretSet) && !keySet) {
        langKey = L"YouTube.YTAPI\\HintMissingKey";
        isValid = false;
    } else if ((clientIDSet || clientSecretSet) && (!clientIDSet || !clientSecretSet)) {
        langKey = L"YouTube.YTAPI\\HintIncompleteOAuth";
        isValid = false;
    } else if (!clientIDSet && !clientSecretSet && keySet) {
        langKey = L"YouTube.YTAPI\\HintSetKey";
    } else {
        langKey = L"YouTube.YTAPI\\HintSetAll";
    }
    SetDlgItemText(hwnd, IDC_YTAPIUSAGE, Plugin::instance()->Lang(langKey).c_str());
    EnableWindow(GetDlgItem(hwnd, IDOK), isValid);
}
