/************************************************/
/*                                              */
/*          AIMP Programming Interface          */
/*               v3.60 build 1426               */
/*                                              */
/*                Artem Izmaylov                */
/*                (C) 2006-2015                 */
/*                 www.aimp.ru                  */
/*              ICQ: 345-908-513                */
/*            Mail: support@aimp.ru             */
/*                                              */
/************************************************/

#ifndef apiTagEditorH
#define apiTagEditorH

#include <windows.h>
#include <unknwn.h>
#include "apiObjects.h"
#include "apiFileManager.h"

static const GUID IID_IAIMPFileTag 		        = {0x41494D50, 0x4669, 0x6C65, 0x54, 0x61, 0x67, 0x00, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPFileTagEditor        = {0x41494D50, 0x4669, 0x6C65, 0x54, 0x61, 0x67, 0x45, 0x64, 0x69, 0x74, 0x00};
static const GUID IID_IAIMPServiceFileTagEditor = {0x41494D50, 0x5372, 0x7654, 0x61, 0x67, 0x45, 0x64, 0x69, 0x74, 0x00, 0x00};

// PropertyID for the IAIMPFileTag
const int AIMP_FILETAG_PROPID_BASE             = 100;
const int AIMP_FILETAG_PROPID_TAG_ID           = AIMP_FILETAG_PROPID_BASE + 1;
const int AIMP_FILETAG_PROPID_DELETE_ON_SAVING = AIMP_FILETAG_PROPID_BASE + 2;

// IDs for IAIMPFileTag.AIMP_FILETAG_PROPID_TAG_ID
const int AIMP_FILETAG_ID_CUSTOM = 0;
const int AIMP_FILETAG_ID_APEv2  = 1;
const int AIMP_FILETAG_ID_ID3v1  = 2;
const int AIMP_FILETAG_ID_ID3v2  = 3;
const int AIMP_FILETAG_ID_MP4    = 4;
const int AIMP_FILETAG_ID_VORBIS = 5;
const int AIMP_FILETAG_ID_WMA    = 6;

/* IAIMPFileTag */

class IAIMPFileTag: public IAIMPFileInfo
{
	// Nothing
};

/* IAIMPFileTagEditor */

class IAIMPFileTagEditor: public IUnknown
{
	public:
		// Info
		virtual HRESULT WINAPI GetMixedInfo(IAIMPFileInfo **Info) = 0;
		virtual HRESULT WINAPI GetTag(int Index, REFIID IID, void **Obj) = 0;
		virtual int WINAPI GetTagCount() = 0;
		virtual HRESULT WINAPI SetToAll(IAIMPFileInfo *Info) = 0;
		// Save
		virtual HRESULT WINAPI Save() = 0;
};

/* IAIMPServiceFileTagEditor */

class IAIMPServiceFileTagEditor: public IUnknown
{
	public:
		virtual HRESULT WINAPI EditFile(IUnknown *Source, REFIID IID, void **Obj) = 0;
		virtual HRESULT WINAPI EditTag(IUnknown *Source, int TagID, REFIID IID, void **Obj) = 0;
};

#endif // !apiTagEditorH
