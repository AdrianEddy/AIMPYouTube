/************************************************/
/*                                              */
/*          AIMP Programming Interface          */
/*               v3.60 build 1473               */
/*                                              */
/*                Artem Izmaylov                */
/*                (C) 2006-2015                 */
/*                 www.aimp.ru                  */
/*              ICQ: 345-908-513                */
/*            Mail: support@aimp.ru             */
/*                                              */
/************************************************/

#ifndef apiAlbumArtH
#define apiAlbumArtH

#include <windows.h>
#include <unknwn.h>
#include "apiObjects.h"
#include "apiCore.h"
#include "apiFileManager.h"

static const GUID IID_IAIMPExtensionAlbumArtCatalog   = {0x41494D50, 0x4578, 0x7441, 0x6C, 0x62, 0x41, 0x72, 0x74, 0x43, 0x61, 0x74};
static const GUID IID_IAIMPExtensionAlbumArtCatalog2  = {0x41494D50, 0x4578, 0x416C, 0x62, 0x41, 0x72, 0x74, 0x43, 0x61, 0x74, 0x32};
static const GUID IID_IAIMPExtensionAlbumArtProvider  = {0x41494D50, 0x4578, 0x7441, 0x6C, 0x62, 0x41, 0x72, 0x74, 0x50, 0x72, 0x76};
static const GUID IID_IAIMPExtensionAlbumArtProvider2 = {0x41494D50, 0x4578, 0x416C, 0x62, 0x41, 0x72, 0x74, 0x50, 0x72, 0x76, 0x32};
static const GUID IID_IAIMPServiceAlbumArt = {0x41494D50, 0x5372, 0x7641, 0x6C, 0x62, 0x41, 0x72, 0x74, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPServiceAlbumArtCache = {0x4941494D, 0x5053, 0x7276, 0x41, 0x6C, 0x62, 0x41, 0x72, 0x74, 0x43, 0x68};

const int AIMP_ALBUMART_PROVIDER_CATEGORY_MASK     = 0xF;
// Providers Categories
const int AIMP_ALBUMART_PROVIDER_CATEGORY_TAGS     = 0;
const int AIMP_ALBUMART_PROVIDER_CATEGORY_FILE     = 1;
const int AIMP_ALBUMART_PROVIDER_CATEGORY_INTERNET = 2;

// PropIDs for IAIMPPropertyList in IAIMPExtensionAlbumArtLocalFinder.Get
const int AIMP_SERVICE_ALBUMART_PROPID_FIND_IN_FILES                  = 1;
const int AIMP_SERVICE_ALBUMART_PROPID_FIND_IN_FILES_MASKS            = 2;
const int AIMP_SERVICE_ALBUMART_PROPID_FIND_IN_FILES_EXTS             = 3;
const int AIMP_SERVICE_ALBUMART_PROPID_FIND_IN_INTERNET               = 4;
const int AIMP_SERVICE_ALBUMART_PROPID_FIND_IN_INTERNET_MAX_FILE_SIZE = 5;

// Flags for IAIMPServiceAlbumArt.Get
const int AIMP_SERVICE_ALBUMART_FLAGS_IGNORECACHE = 1;
const int AIMP_SERVICE_ALBUMART_FLAGS_ORIGINAL    = 2;
const int AIMP_SERVICE_ALBUMART_FLAGS_WAITFOR     = 4;

typedef void (CALLBACK TAIMPServiceAlbumArtReceiveProc)(IAIMPImage *image, IAIMPImageContainer *imageContainer, void *UserData);

/* IAIMPExtensionAlbumArtCatalog */

class IAIMPExtensionAlbumArtCatalog: public IUnknown
{
	public:
		virtual HRESULT WINAPI GetIcon(HICON *Image) = 0;
		virtual HRESULT WINAPI GetName(IAIMPString **Name) = 0;
		virtual HRESULT WINAPI Show(IAIMPString *FileURI, IAIMPString *Artist, IAIMPString *Album, IAIMPImageContainer **Image) = 0;
};

/* IAIMPExtensionAlbumArtCatalog2 */

class IAIMPExtensionAlbumArtCatalog2: public IAIMPExtensionAlbumArtCatalog
{
	public:
		virtual HRESULT WINAPI Show2(IAIMPFileInfo *FileInfo, IAIMPImageContainer **Image) = 0;
};

/* IAIMPExtensionAlbumArtProvider */

class IAIMPExtensionAlbumArtProvider: public IUnknown
{
	public:
		virtual HRESULT WINAPI Get(IAIMPString *FileURI, IAIMPString *Artist, IAIMPString *Album, IAIMPPropertyList *Options, IAIMPImageContainer **Image) = 0;
		virtual DWORD WINAPI GetCategory() = 0;
};

/* IAIMPExtensionAlbumArtProvider2 */

class IAIMPExtensionAlbumArtProvider2: public IAIMPExtensionAlbumArtProvider
{
	public:
		virtual HRESULT WINAPI Get2(IAIMPFileInfo *FileInfo, IAIMPPropertyList *Options, IAIMPImageContainer **Image) = 0;
};

/* IAIMPServiceAlbumArt */

class IAIMPServiceAlbumArt: public IUnknown
{
	public:
		virtual HRESULT WINAPI Get(IAIMPString *FileURI, IAIMPString *Artist, IAIMPString *Album, DWORD Flags, TAIMPServiceAlbumArtReceiveProc *CallbackProc, void *UserData, void **TaskID) = 0;
		virtual HRESULT WINAPI Get2(IAIMPFileInfo *FileInfo, DWORD Flags, TAIMPServiceAlbumArtReceiveProc *CallbackProc, void *UserData, void **TaskID) = 0;
		virtual HRESULT WINAPI Cancel(void *TaskID, DWORD Flags) = 0;
};

/* IAIMPServiceAlbumArtCache */

class IAIMPServiceAlbumArtCache: public IUnknown
{
	public:
		virtual HRESULT WINAPI Flush(IAIMPString *Artist, IAIMPString *Album) = 0;
		virtual HRESULT WINAPI Flush2(IAIMPString *FileURI) = 0;
		virtual HRESULT WINAPI FlushAll() = 0;
};

#endif // !apiAlbumArtH
