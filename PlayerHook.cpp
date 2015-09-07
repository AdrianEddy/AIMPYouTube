#include "PlayerHook.h"
#include <string>
#include "Tools.h"
#include "YouTubeAPI.h"

HRESULT WINAPI PlayerHook::OnCheckURL(IAIMPString *URL, BOOL **Handled) {
    if (wcsstr(URL->GetData(), L"youtube.com") == 0)
        return E_FAIL;

    std::wstring id = Tools::TrackIdFromUrl(URL->GetData());
    std::wstring stream_url = YouTubeAPI::GetStreamUrl(id);
    URL->SetData(const_cast<wchar_t *>(stream_url.c_str()), stream_url.size());

    *Handled = new BOOL(1);
    return S_OK;
}
