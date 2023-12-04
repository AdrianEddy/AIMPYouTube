/************************************************/
/*                                              */
/*          AIMP Programming Interface          */
/*               v5.30 build 2500               */
/*                                              */
/*                Artem Izmaylov                */
/*                (C) 2006-2023                 */
/*                 www.aimp.ru                  */
/*               support@aimp.ru                */
/*                                              */
/************************************************/

#ifndef apiLyricsH
#define apiLyricsH

#include <windows.h>
#include <unknwn.h>
#include "apiObjects.h"
#include "apiFileManager.h"
#include "apiThreading.h"

static const GUID IID_IAIMPLyrics = {0x41494D50, 0x4C79, 0x7269, 0x63, 0x73, 0x46, 0x69, 0x6C, 0x65, 0x00, 0x00};
static const GUID IID_IAIMPExtensionLyricsProvider = {0x41494D50, 0x4578, 0x744C, 0x79, 0x72, 0x69, 0x78, 0x50, 0x72, 0x76, 0x00};
static const GUID IID_IAIMPServiceLyrics = {0x41494D50, 0x5372, 0x764C, 0x79, 0x72, 0x69, 0x78, 0x00, 0x00, 0x00, 0x00};

// PropertyID for the IAIMPLyrics
const int AIMP_LYRICS_PROPID_TEXT     = 1;
const int AIMP_LYRICS_PROPID_TYPE     = 2;
const int AIMP_LYRICS_PROPID_LYRICIST = 3;
const int AIMP_LYRICS_PROPID_OFFSET   = 4;
const int AIMP_LYRICS_PROPID_ALBUM    = 5;
const int AIMP_LYRICS_PROPID_TITLE    = 6;
const int AIMP_LYRICS_PROPID_CREATOR  = 7;
const int AIMP_LYRICS_PROPID_APP      = 8;
const int AIMP_LYRICS_PROPID_APPVER   = 9;

// Lyrics Type
const int AIMP_LYRICS_TYPE_UNKNOWN   = 0;
const int AIMP_LYRICS_TYPE_UNSYNCED  = 1;
const int AIMP_LYRICS_TYPE_SYNCED    = 2;

// IAIMPLyrics's File Format
const int AIMP_LYRICS_FORMAT_TXT = 0;
const int AIMP_LYRICS_FORMAT_LRC = 1;
const int AIMP_LYRICS_FORMAT_SRT = 2;

// Flags for IAIMPServiceLyrics.Get
const int AIMP_SERVICE_LYRICS_FLAGS_NOCACHE = 1;
const int AIMP_SERVICE_LYRICS_FLAGS_WAITFOR = 4;

// IAIMPExtensionLyricsProvider.GetCategory
const int AIMP_LYRICS_PROVIDER_CATEGORY_FILE     = 1;
const int AIMP_LYRICS_PROVIDER_CATEGORY_INTERNET = 2;

/* IAIMPVisualExtension */

class IAIMPLyrics: public IAIMPPropertyList
{
	public:
		virtual HRESULT WINAPI Assign(IAIMPLyrics* Source) = 0;
		virtual HRESULT WINAPI Clone(IAIMPLyrics** Target) = 0;
		//
		virtual HRESULT WINAPI Add(int TimeStart, int TimeFinish, IAIMPString* Text) = 0;
		virtual HRESULT WINAPI Delete(int Index) = 0;
		virtual HRESULT WINAPI Find(int Time, int* Index, IAIMPString** Text) = 0;
		virtual HRESULT WINAPI Get(int Index, int* TimeStart, int* TimeFinish, IAIMPString** Text) = 0;
		virtual HRESULT WINAPI GetCount(int* Value) = 0;
		// I/O
		virtual HRESULT WINAPI LoadFromFile(IAIMPString* FileURI) = 0;
		virtual HRESULT WINAPI LoadFromStream(IAIMPStream* Stream, int Format) = 0;
		virtual HRESULT WINAPI LoadFromString(IAIMPString* String, int Format) = 0;
		virtual HRESULT WINAPI SaveToFile(IAIMPString* FileURI) = 0;
		virtual HRESULT WINAPI SaveToStream(IAIMPStream* Stream, int Format) = 0;
		virtual HRESULT WINAPI SaveToString(IAIMPString** String, int Format) = 0;
};

typedef void (CALLBACK TAIMPServiceLyricsReceiveProc)(IAIMPLyrics *Lyrics, void *UserData);

/* IAIMPExtensionLyricsProvider */

class IAIMPExtensionLyricsProvider: public IUnknown
{
	public:
		virtual HRESULT WINAPI Get(IAIMPTaskOwner* Owner, IAIMPFileInfo* FileInfo, DWORD Flags, IAIMPLyrics* Lyrics) = 0;
		virtual DWORD WINAPI GetCategory() = 0;
};


/* IAIMPServiceLyrics */

class IAIMPServiceLyrics: public IUnknown
{
	public:
		virtual HRESULT WINAPI Get(IAIMPFileInfo* FileInfo, DWORD Flags, TAIMPServiceLyricsReceiveProc *CallbackProc, void *UserData, void **TaskID) = 0;
		virtual HRESULT WINAPI Cancel(void* TaskID, DWORD Flags) = 0;
};

#endif // !apiLyricsH