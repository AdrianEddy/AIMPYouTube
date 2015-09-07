/************************************************/
/*                                              */
/*          AIMP Programming Interface          */
/*               v3.60 build 1455               */
/*                                              */
/*                Artem Izmaylov                */
/*                (C) 2006-2015                 */
/*                 www.aimp.ru                  */
/*              ICQ: 345-908-513                */
/*            Mail: support@aimp.ru             */
/*                                              */
/************************************************/

#ifndef apiFileManagerH
#define apiFileManagerH

#include <windows.h>
#include <unknwn.h>
#include "apiObjects.h"

static const GUID IID_IAIMPFileInfo = {0x41494D50, 0x4669, 0x6C65, 0x49, 0x6E, 0x66, 0x6F, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPExtensionFileExpander = {0x41494D50, 0x4578, 0x7446, 0x69, 0x6C, 0x65, 0x45, 0x78, 0x70, 0x64, 0x72};
static const GUID IID_IAIMPExtensionFileFormat = {0x41494D50, 0x4578, 0x7446, 0x69, 0x6C, 0x65, 0x46, 0x6D, 0x74, 0x00, 0x00};
static const GUID IID_IAIMPExtensionFileInfoProvider = {0x41494D50, 0x4578, 0x7446, 0x69, 0x6C, 0x65, 0x49, 0x6E, 0x66, 0x6F, 0x00};
static const GUID IID_IAIMPExtensionFileInfoProviderEx = {0x41494D50, 0x4578, 0x7446, 0x69, 0x6C, 0x65, 0x49, 0x6E, 0x66, 0x6F, 0x45};
static const GUID IID_IAIMPVirtualFile = {0x41494D50, 0x5669, 0x7274, 0x75, 0x61, 0x6C, 0x46, 0x69, 0x6C, 0x65, 0x00};
static const GUID IID_IAIMPServiceFileManager = {0x41494D50, 0x5372, 0x7646, 0x69, 0x6C, 0x65, 0x4D, 0x61, 0x6E, 0x00, 0x00};
static const GUID IID_IAIMPServiceFileFormats = {0x41494D50, 0x5372, 0x7646, 0x69, 0x6C, 0x65, 0x46, 0x6D, 0x74, 0x73, 0x00};
static const GUID IID_IAIMPServiceFileInfo = {0x41494D50, 0x5372, 0x7646, 0x69, 0x6C, 0x65, 0x49, 0x6E, 0x66, 0x6F, 0x00};
static const GUID IID_IAIMPServiceFileInfoFormatter = {0x41494D50, 0x5372, 0x7646, 0x6C, 0x49, 0x6E, 0x66, 0x46, 0x6D, 0x74, 0x00};
static const GUID IID_IAIMPServiceFileInfoFormatterUtils = {0x41494D50, 0x5372, 0x7646, 0x6C, 0x49, 0x6E, 0x66, 0x46, 0x6D, 0x74, 0x55};
static const GUID IID_IAIMPServiceFileStreaming = {0x41494D50, 0x5372, 0x7646, 0x69, 0x6C, 0x65, 0x53, 0x74, 0x72, 0x6D, 0x00};

// PropertyID for the IAIMPFileInfo
const int AIMP_FILEINFO_PROPID_CUSTOM            = 0;
const int AIMP_FILEINFO_PROPID_ALBUM             = 1;
const int AIMP_FILEINFO_PROPID_ALBUMART          = 2;
const int AIMP_FILEINFO_PROPID_ALBUMARTIST       = 3;
const int AIMP_FILEINFO_PROPID_ALBUMGAIN         = 4;
const int AIMP_FILEINFO_PROPID_ALBUMPEAK         = 5;
const int AIMP_FILEINFO_PROPID_ARTIST            = 6;
const int AIMP_FILEINFO_PROPID_BITRATE           = 7;
const int AIMP_FILEINFO_PROPID_BPM               = 8;
const int AIMP_FILEINFO_PROPID_CHANNELS          = 9;
const int AIMP_FILEINFO_PROPID_COMMENT           = 10;
const int AIMP_FILEINFO_PROPID_COMPOSER          = 11;
const int AIMP_FILEINFO_PROPID_COPYRIGHT         = 12;
const int AIMP_FILEINFO_PROPID_CUESHEET          = 13;
const int AIMP_FILEINFO_PROPID_DATE              = 14;
const int AIMP_FILEINFO_PROPID_DISKNUMBER        = 15;
const int AIMP_FILEINFO_PROPID_DISKTOTAL         = 16;
const int AIMP_FILEINFO_PROPID_DURATION          = 17;
const int AIMP_FILEINFO_PROPID_FILENAME          = 18;
const int AIMP_FILEINFO_PROPID_FILESIZE          = 19;
const int AIMP_FILEINFO_PROPID_GENRE             = 20;
const int AIMP_FILEINFO_PROPID_LYRICS            = 21;
const int AIMP_FILEINFO_PROPID_MARK              = 22;
const int AIMP_FILEINFO_PROPID_PUBLISHER         = 23;
const int AIMP_FILEINFO_PROPID_SAMPLERATE        = 24;
const int AIMP_FILEINFO_PROPID_TITLE             = 25;
const int AIMP_FILEINFO_PROPID_TRACKGAIN         = 26;
const int AIMP_FILEINFO_PROPID_TRACKNUMBER       = 27;
const int AIMP_FILEINFO_PROPID_TRACKPEAK         = 28;
const int AIMP_FILEINFO_PROPID_TRACKTOTAL        = 29;
const int AIMP_FILEINFO_PROPID_URL               = 30;
const int AIMP_FILEINFO_PROPID_BITDEPTH          = 31;
const int AIMP_FILEINFO_PROPID_CODEC             = 32;
const int AIMP_FILEINFO_PROPID_STAT_ADDINGDATE    = 40;
const int AIMP_FILEINFO_PROPID_STAT_LASTPLAYDATE  = 41;
const int AIMP_FILEINFO_PROPID_STAT_MARK	      = 42;
const int AIMP_FILEINFO_PROPID_STAT_PLAYCOUNT     = 43;
const int AIMP_FILEINFO_PROPID_STAT_RATING        = 44;

// PropertyID for the IAIMPVirtualFile
const int AIMP_VIRTUALFILE_PROPID_FILEURI          = 0;
const int AIMP_VIRTUALFILE_PROPID_AUDIOSOURCEFILE  = 1;
const int AIMP_VIRTUALFILE_PROPID_CLIPSTART        = 2;
const int AIMP_VIRTUALFILE_PROPID_CLIPFINISH       = 3;
const int AIMP_VIRTUALFILE_PROPID_INDEXINSET       = 4;
const int AIMP_VIRTUALFILE_PROPID_FILEFORMAT       = 5; 

// Flags for the IAIMPServiceFileFormats and IAIMPExtensionFileFormat
const int AIMP_SERVICE_FILEFORMATS_CATEGORY_AUDIO     = 1;
const int AIMP_SERVICE_FILEFORMATS_CATEGORY_PLAYLISTS = 2;

// Flags for the IAIMPServiceFileManager.CreateFileStream
const int AIMP_SERVICE_FILESTREAMING_FLAG_CREATENEW   = 1;
const int AIMP_SERVICE_FILESTREAMING_FLAG_READWRITE   = 2;
const int AIMP_SERVICE_FILESTREAMING_FLAG_MAPTOMEMORY = 4;

// Flags for the IAIMPServiceFileInfo.GetFileInfoXXX
const int AIMP_SERVICE_FILEINFO_FLAG_DONTUSEAUDIODECODERS = 1;

/* IAIMPFileInfo */

class IAIMPFileInfo: public IAIMPPropertyList
{
	public:
		virtual HRESULT WINAPI Assign(IAIMPFileInfo *Source) = 0;
		virtual HRESULT WINAPI Clone(IAIMPFileInfo **Info) = 0;
};

/* IAIMPVirtualFile */

class IAIMPVirtualFile: public IAIMPPropertyList
{
	public:
		virtual HRESULT WINAPI CreateStream(IAIMPStream **Stream) = 0;
		virtual HRESULT WINAPI GetFileInfo(IAIMPFileInfo *Info) = 0;
		virtual HRESULT WINAPI IsExists() = 0;
		virtual HRESULT WINAPI IsInSameStream(IAIMPVirtualFile *VirtualFile) = 0;
		virtual HRESULT WINAPI Synchronize() = 0;
};

/* IAIMPExtensionFileExpander */

class IAIMPExtensionFileExpander: public IUnknown
{
	public:
		virtual HRESULT WINAPI Expand(IAIMPString *FileName, IAIMPObjectList **List, IAIMPProgressCallback *ProgressCallback) = 0;
};

/* IAIMPExtensionFileFormat */

class IAIMPExtensionFileFormat: public IUnknown
{
	public:
		virtual HRESULT WINAPI GetDescription(IAIMPString **S) = 0;
		virtual HRESULT WINAPI GetExtList(IAIMPString **S) = 0;
		virtual HRESULT WINAPI GetFlags(DWORD **S) = 0;
};

/* IAIMPExtensionFileInfoProvider */

class IAIMPExtensionFileInfoProvider: public IUnknown
{
	public:
		virtual HRESULT WINAPI GetFileInfo(IAIMPString *FileURI, IAIMPFileInfo *Info) = 0;
};

/* IAIMPExtensionFileInfoProviderEx */

class IAIMPExtensionFileInfoProviderEx: public IUnknown
{
	public:
		virtual HRESULT WINAPI GetFileInfo(IAIMPStream *Stream, IAIMPFileInfo *Info) = 0;
};

/* IAIMPServiceFileManager */

class IAIMPServiceFileManager: public IUnknown
{
	// nothing
};

/* IAIMPServiceFileFormats */

class IAIMPServiceFileFormats: public IUnknown
{
	public:
		virtual HRESULT WINAPI GetFormats(DWORD Flags, IAIMPString **S) = 0;
		virtual HRESULT WINAPI IsSupported(IAIMPString *FileName, DWORD Flags) = 0;
};

/* IAIMPServiceFileInfo */

class IAIMPServiceFileInfo: public IUnknown
{
	public:
		// File Info
		virtual HRESULT WINAPI GetFileInfoFromFileURI(IAIMPString *FileURI, DWORD Flags, IAIMPFileInfo *Info) = 0;
		virtual HRESULT WINAPI GetFileInfoFromStream(IAIMPStream *Stream, DWORD Flags, IAIMPFileInfo *Info) = 0;
		// Virtual Files
		virtual HRESULT WINAPI GetVirtualFile(IAIMPString *FileURI, DWORD Flags, IAIMPVirtualFile **Info) = 0;
};

/* IAIMPServiceFileInfoFormatter */

class IAIMPServiceFileInfoFormatter: public IUnknown
{
	public:
		virtual HRESULT WINAPI Format(IAIMPString *Template, IAIMPFileInfo *FileInfo, int Reserved, IUnknown *AdditionalInfo, IAIMPString **FormattedResult) = 0;
};

/* IAIMPServiceFileInfoFormatterUtils */

class IAIMPServiceFileInfoFormatterUtils: public IUnknown
{
	public:
		virtual HRESULT WINAPI ShowMacrosLegend(RECT ScreenTarget, int Reserved, IUnknown *EventsHandler) = 0;
};

/* IAIMPServiceFileStreaming */

class IAIMPServiceFileStreaming: public IUnknown
{
	public:
		virtual HRESULT WINAPI CreateStreamForFile(IAIMPString *FileName, DWORD Flags, const INT64 Offset, const INT64 Size, IAIMPStream **Stream) = 0;
		virtual HRESULT WINAPI CreateStreamForFileURI(IAIMPString *FileURI, IAIMPVirtualFile **VirtualFile, IAIMPStream **Stream) = 0;
};

#endif // !apiFileManagerH