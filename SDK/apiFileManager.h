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
static const GUID IID_IAIMPServiceFileURI = {0x41494D50, 0x5372, 0x7646, 0x69, 0x6C, 0x65, 0x55, 0x52, 0x49, 0x00, 0x00};
static const GUID IID_IAIMPExtensionFileSystem = {0x41494D50, 0x4578, 0x7446, 0x53, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPFileSystem = {0x41494D50, 0x4653, 0x0000, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPFileSystemCommandCopyToClipboard = {0x41465343, 0x6D64, 0x436F, 0x70, 0x79, 0x32, 0x43, 0x6C, 0x70, 0x62, 0x64};
static const GUID IID_IAIMPFileSystemCommandDelete = {0x41465343, 0x6D64, 0x4465, 0x6C, 0x65, 0x74, 0x65, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPFileSystemCommandDropSource = {0x41465343, 0x6D64, 0x4472, 0x6F, 0x70, 0x53, 0x72, 0x63, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPFileSystemCommandFileExists = {0x41465343, 0x6D64, 0x4669, 0x6C, 0x65, 0x45, 0x78, 0x69, 0x73, 0x74, 0x70};
static const GUID IID_IAIMPFileSystemCommandFileInfo = { 0x41494D50, 0x4578, 0x7446, 0x69, 0x6C, 0x65, 0x49, 0x6E, 0x66, 0x6F, 0x00};
static const GUID IID_IAIMPFileSystemCommandOpenFileFolder = {0x41465343, 0x6D64, 0x4669, 0x6C, 0x65, 0x46, 0x6C, 0x64, 0x72, 0x00, 0x00};
static const GUID IID_IAIMPFileSystemCommandStreaming = {0x41465343, 0x6D64, 0x5374, 0x72, 0x65, 0x61, 0x6D, 0x69, 0x6E, 0x67, 0x00};
static const GUID IID_IAIMPServiceFileSystems = {0x41494D50, 0x5372, 0x7646, 0x53, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPServiceFileURI2 = {0x41494D50, 0x5372, 0x7646, 0x69, 0x6C, 0x65, 0x55, 0x52, 0x49, 0x32, 0x00};

// PropertyID for the IAIMPFileInfo
const int AIMP_FILEINFO_PROPID_CUSTOM            = 0; // IUnknown
const int AIMP_FILEINFO_PROPID_ALBUM             = 1; // IAIMPString
const int AIMP_FILEINFO_PROPID_ALBUMART          = 2; // IAIMPImage / IAIMPImageContainer
const int AIMP_FILEINFO_PROPID_ALBUMARTIST       = 3; // IAIMPString
const int AIMP_FILEINFO_PROPID_ALBUMGAIN         = 4; // Float
const int AIMP_FILEINFO_PROPID_ALBUMPEAK         = 5; // Float
const int AIMP_FILEINFO_PROPID_ARTIST            = 6; // IAIMPString
const int AIMP_FILEINFO_PROPID_BITRATE           = 7; // Int32
const int AIMP_FILEINFO_PROPID_BPM               = 8; // Int32
const int AIMP_FILEINFO_PROPID_CHANNELS          = 9; // Int32
const int AIMP_FILEINFO_PROPID_COMMENT           = 10; // IAIMPString
const int AIMP_FILEINFO_PROPID_COMPOSER          = 11; // IAIMPString
const int AIMP_FILEINFO_PROPID_COPYRIGHT         = 12; // IAIMPString
const int AIMP_FILEINFO_PROPID_CUESHEET          = 13; // IAIMPString
const int AIMP_FILEINFO_PROPID_DATE              = 14; // IAIMPString
const int AIMP_FILEINFO_PROPID_DISKNUMBER        = 15; // IAIMPString
const int AIMP_FILEINFO_PROPID_DISKTOTAL         = 16; // IAIMPString
const int AIMP_FILEINFO_PROPID_DURATION          = 17; // Float
const int AIMP_FILEINFO_PROPID_FILENAME          = 18; // IAIMPString
const int AIMP_FILEINFO_PROPID_FILESIZE          = 19; // Int64
const int AIMP_FILEINFO_PROPID_GENRE             = 20; // IAIMPString
const int AIMP_FILEINFO_PROPID_LYRICS            = 21; // IAIMPString
const int AIMP_FILEINFO_PROPID_PUBLISHER         = 23; // Int32
const int AIMP_FILEINFO_PROPID_SAMPLERATE        = 24; // Int32
const int AIMP_FILEINFO_PROPID_TITLE             = 25; // IAIMPString
const int AIMP_FILEINFO_PROPID_TRACKGAIN         = 26; // Float
const int AIMP_FILEINFO_PROPID_TRACKNUMBER       = 27; // IAIMPString
const int AIMP_FILEINFO_PROPID_TRACKPEAK         = 28; // Float
const int AIMP_FILEINFO_PROPID_TRACKTOTAL        = 29; // IAIMPString
const int AIMP_FILEINFO_PROPID_URL               = 30; // IAIMPString
const int AIMP_FILEINFO_PROPID_BITDEPTH          = 31; // Int32
const int AIMP_FILEINFO_PROPID_CODEC             = 32; // IAIMPString
const int AIMP_FILEINFO_PROPID_CONDUCTOR         = 33; // IAIMPString
const int AIMP_FILEINFO_PROPID_MOOD              = 34; // IAIMPString
const int AIMP_FILEINFO_PROPID_CATALOG           = 35; // IAIMPString
const int AIMP_FILEINFO_PROPID_ISRC              = 36; // IAIMPString
const int AIMP_FILEINFO_PROPID_LYRICIST          = 37; // IAIMPString
const int AIMP_FILEINFO_PROPID_ENCODEDBY         = 38; // IAIMPString
const int AIMP_FILEINFO_PROPID_RATING            = 39; // Int32
const int AIMP_FILEINFO_PROPID_ML_ADDINGDATE      = 40; // Float (TDateTime)
const int AIMP_FILEINFO_PROPID_ML_LASTPLAYDATE    = 41; // Float (TDateTime)
const int AIMP_FILEINFO_PROPID_ML_MARK            = 42; // Int32
const int AIMP_FILEINFO_PROPID_ML_PLAYCOUNT       = 43; // Int32
const int AIMP_FILEINFO_PROPID_ML_RATING          = 44; // Float
const int AIMP_FILEINFO_PROPID_ML_DISPLAYING_MARK = 22; // Float
const int AIMP_FILEINFO_PROPID_ML_LABELS		  = 45; // IAIMPString, values divided by ";"
const int AIMP_FILEINFO_PROPID_KEY                = 46; // IAIMPString

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
const int AIMP_SERVICE_FILESTREAMING_FLAG_READ        = 0;
const int AIMP_SERVICE_FILESTREAMING_FLAG_READWRITE   = 2;
const int AIMP_SERVICE_FILESTREAMING_FLAG_BUFFERED    = 4;

// Flags for the IAIMPServiceFileInfo.GetFileInfoXXX
const int AIMP_SERVICE_FILEINFO_FLAG_DONTUSEAUDIODECODERS = 1;

// Flags for the IAIMPServiceFileURI.ChangeFileExt and IAIMPServiceFileURI.ExtractFileExt
const int AIMP_SERVICE_FILEURI_FLAG_DOUBLE_EXTS = 1;
const int AIMP_SERVICE_FILEURI_FLAG_PART_EXT    = 2;

// Property IDs for IAIMPExtensionFileSystem
const int AIMP_FILESYSTEM_PROPID_SCHEME = 1;
const int AIMP_FILESYSTEM_PROPID_READONLY = 2;

// Factory IDs for IAIMPServiceFileInfoFormatter and IAIMPServiceFileInfoFormatterUtils
const int AIMP_FILEINFO_FORMATTER_ID_BASIC   = 0;
const int AIMP_FILEINFO_FORMATTER_ID_PLAYING = 1;

//----------------------------------------------------------------------------------------------------------------------
// Common
//----------------------------------------------------------------------------------------------------------------------

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

#pragma pack(push, 1)
struct TAIMPFileAttributes 
{
	DWORD Attributes;
	DOUBLE TimeCreation;
	DOUBLE TimeLastAccess;
	DOUBLE TimeLastWrite;
	INT64 Reserved0;
	INT64 Reserved1;
	INT64 Reserved2;
};
#pragma pack(pop)

//----------------------------------------------------------------------------------------------------------------------
// FileSystem Commands
//----------------------------------------------------------------------------------------------------------------------

/* IAIMPFileSystemCustomFileCommand */

class IAIMPFileSystemCustomFileCommand : public IUnknown
{
	public:
		virtual HRESULT WINAPI CanProcess(IAIMPString* FileName) = 0;
		virtual HRESULT WINAPI Process(IAIMPString* FileName) = 0;
};

/* IAIMPFileSystemCommandCopyToClipboard */

class IAIMPFileSystemCommandCopyToClipboard : public IUnknown 
{
	public:
		virtual HRESULT WINAPI CopyToClipboard(IAIMPObjectList* Files) = 0;
};

/* IAIMPFileSystemCommandDelete */

class IAIMPFileSystemCommandDelete : public IAIMPFileSystemCustomFileCommand
{
};

/* IAIMPFileSystemCommandDropSource */

class IAIMPFileSystemCommandDropSource : public IUnknown
{
	public:
		virtual HRESULT WINAPI CreateStream(IAIMPString* FileName, IAIMPStream** Stream) = 0;
};

/* IAIMPFileSystemCommandFileInfo */

class IAIMPFileSystemCommandFileInfo : public IUnknown 
{
	public:
		virtual HRESULT WINAPI GetFileAttrs(IAIMPString* FileName, TAIMPFileAttributes* Attrs) = 0;
		virtual HRESULT WINAPI GetFileSize(IAIMPString* FileName, INT64* Size) = 0;
		virtual HRESULT WINAPI IsFileExists(IAIMPString* FileName) = 0;
};

/* IAIMPFileSystemCommandOpenFileFolder */

class IAIMPFileSystemCommandOpenFileFolder : public IAIMPFileSystemCustomFileCommand
{

};

/* IAIMPFileSystemCommandStreaming */

class IAIMPFileSystemCommandStreaming : public IUnknown
{
	public:
		virtual HRESULT WINAPI CreateStream(IAIMPString* FileName, const INT64 Offset, const INT64 Size, DWORD Flags, IAIMPStream** Stream) = 0;
};
//----------------------------------------------------------------------------------------------------------------------
// Extensions
//----------------------------------------------------------------------------------------------------------------------

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
		virtual HRESULT WINAPI GetFlags(DWORD *S) = 0;
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

/* IAIMPExtensionFileSystem */

class IAIMPExtensionFileSystem : public IAIMPPropertyList
{

};

//----------------------------------------------------------------------------------------------------------------------
// Services
//----------------------------------------------------------------------------------------------------------------------

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
		virtual HRESULT WINAPI Format(IAIMPString *Template, IAIMPFileInfo *FileInfo, int ID, IUnknown *AdditionalInfo, IAIMPString **FormattedResult) = 0;
};

/* IAIMPServiceFileInfoFormatterUtils */

class IAIMPServiceFileInfoFormatterUtils: public IUnknown
{
	public:
		virtual HRESULT WINAPI ShowMacrosLegend(RECT ScreenTarget, int ID, IUnknown *EventsHandler) = 0;
};

/* IAIMPServiceFileStreaming */

class IAIMPServiceFileStreaming: public IUnknown
{
	public:
		virtual HRESULT WINAPI CreateStreamForFile(IAIMPString *FileName, DWORD Flags, const INT64 Offset, const INT64 Size, IAIMPStream **Stream) = 0;
		virtual HRESULT WINAPI CreateStreamForFileURI(IAIMPString *FileURI, IAIMPVirtualFile **VirtualFile, IAIMPStream **Stream) = 0;
};

/* IAIMPServiceFileSystems */

class IAIMPServiceFileSystems : public IUnknown
{
	public:
		virtual HRESULT WINAPI Get(IAIMPString* FileURI, REFIID IID, void **Obj) = 0;
		virtual HRESULT WINAPI GetDefault(REFIID IID, void **Obj) = 0;
};

/* IAIMPServiceFileURI */

class IAIMPServiceFileURI: public IUnknown
{
	public:
		virtual HRESULT WINAPI Build(IAIMPString* ContainerFileName, IAIMPString* PartName, IAIMPString** FileURI) = 0;
		virtual HRESULT WINAPI Parse(IAIMPString* FileURI, IAIMPString** ContainerFileName, IAIMPString** PartName) = 0;

		virtual HRESULT WINAPI ChangeFileExt(IAIMPString** FileURI, IAIMPString* NewExt, DWORD Flags) = 0;
		virtual HRESULT WINAPI ExtractFileExt(IAIMPString* FileURI, IAIMPString** S, DWORD Flags) = 0;
		virtual HRESULT WINAPI ExtractFileName(IAIMPString* FileURI, IAIMPString* S) = 0;
		virtual HRESULT WINAPI ExtractFileParentDirName(IAIMPString* FileURI, IAIMPString** S) = 0;
		virtual HRESULT WINAPI ExtractFileParentName(IAIMPString* FileURI, IAIMPString** S) = 0;
		virtual HRESULT WINAPI ExtractFilePath(IAIMPString* FileURI, IAIMPString** S) = 0;
		virtual HRESULT WINAPI IsURL(IAIMPString* FileURI) = 0;	
};

/* IAIMPServiceFileURI2 */

class IAIMPServiceFileURI2 : public IAIMPServiceFileURI
{
	public:
		virtual HRESULT WINAPI GetScheme(IAIMPString* FileURI, IAIMPString** Scheme) = 0;
};

#endif // !apiFileManagerH