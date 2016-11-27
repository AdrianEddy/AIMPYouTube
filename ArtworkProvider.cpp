#include "ArtworkProvider.h"
#include <string>
#include "AimpHTTP.h"

HRESULT WINAPI ArtworkProvider::Get2(IAIMPFileInfo *FileInfo, IAIMPPropertyList *Options, IAIMPImageContainer **Image) {
    if (!FileInfo || !Options || !Image)
        return E_INVALIDARG;

    IAIMPString *url = nullptr;
    if (SUCCEEDED(FileInfo->GetValueAsObject(AIMP_FILEINFO_PROPID_FILENAME, IID_IAIMPString, reinterpret_cast<void **>(&url)))) {
        auto ti = Tools::TrackInfo(url);
        url->Release();
        if (ti) {
            if (!ti->Artwork.empty()) {
                int maxFileSize = 0;
                if (SUCCEEDED(Options->GetValueAsInt32(AIMP_SERVICE_ALBUMART_PROPID_FIND_IN_INTERNET_MAX_FILE_SIZE, &maxFileSize))) {
                    AimpHTTP::DownloadImage(ti->Artwork, Image, maxFileSize);
                    return *Image ? S_OK : E_FAIL;
                }
            }
        }
    }

    return E_FAIL;
}
