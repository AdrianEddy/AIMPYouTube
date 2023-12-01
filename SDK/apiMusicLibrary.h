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

#ifndef apiMusicLibraryH
#define apiMusicLibraryH

#include <windows.h>
#include <unknwn.h>
#include "apiActions.h"
#include "apiAlbumArt.h"
#include "apiObjects.h"
#include "apiPlayer.h"
#include "apiPlaylists.h"

static const GUID IID_IAIMPServiceMusicLibraryUI = {0x41494D50, 0x5372, 0x764D, 0x4C, 0x55, 0x49, 0x00, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPMLAlbumArtProvider = {0x41494D50, 0x4D4C, 0x416C, 0x62, 0x41, 0x72, 0x74, 0x50, 0x72, 0x76, 0x00};
static const GUID IID_IAIMPMLAlbumArtProvider2 = {0x41494D50, 0x4D4C, 0x416C, 0x62, 0x41, 0x72, 0x74, 0x50, 0x72, 0x76, 0x32};
static const GUID IID_IAIMPMLDataField = {0x41494D50, 0x4D4C, 0x4461, 0x74, 0x61, 0x46, 0x6C, 0x64, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPMLDataFieldDisplayValue = {0x41494D50, 0x4D4C, 0x4461, 0x74, 0x61, 0x46, 0x6C, 0x64, 0x44, 0x56, 0x6C};
static const GUID IID_IAIMPMLDataFieldFilter = {0x41494D50, 0x4D4C, 0x466C, 0x64, 0x46, 0x6C, 0x74, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPMLDataFieldFilterByArray = {0x41494D50, 0x4D4C, 0x466C, 0x64, 0x46, 0x6C, 0x74, 0x41, 0x72, 0x72, 0x00};
static const GUID IID_IAIMPMLDataFilter = {0x41494D50, 0x4D4C, 0x4669, 0x6C, 0x74, 0x65, 0x72, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPMLDataFilterGroup = {0x41494D50, 0x4D4C, 0x466C, 0x74, 0x47, 0x72, 0x70, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPMLDataProvider = {0x41494D50, 0x4D4C, 0x4461, 0x74, 0x61, 0x50, 0x72, 0x76, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPMLDataProvider2 = {0x41494D50, 0x4D4C, 0x4461, 0x74, 0x61, 0x50, 0x72, 0x76, 0x32, 0x00, 0x00};
static const GUID IID_IAIMPMLDataProviderSelection = {0x41494D50, 0x4D4C, 0x4461, 0x74, 0x61, 0x50, 0x72, 0x76, 0x53, 0x65, 0x6C};
static const GUID IID_IAIMPMLDataStorage = {0x41494D50, 0x4D4C, 0x4461, 0x74, 0x61, 0x53, 0x74, 0x67, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPMLDataStorage2 = {0x41494D50, 0x4D4C, 0x4461, 0x74, 0x61, 0x53, 0x74, 0x67, 0x32, 0x00, 0x00};
static const GUID IID_IAIMPMLDataStorageCommandAddFiles = {0x41494D50, 0x4D4C, 0x4453, 0x43, 0x6D, 0x64, 0x41, 0x64, 0x64, 0x00, 0x00};
static const GUID IID_IAIMPMLDataStorageCommandAddFilesDialog = {0x41494D50, 0x4D4C, 0x4453, 0x43, 0x6D, 0x64, 0x41, 0x64, 0x64, 0x44, 0x00};
static const GUID IID_IAIMPMLDataStorageCommandDeleteFiles = {0x41494D50, 0x4D4C, 0x4453, 0x43, 0x6D, 0x64, 0x44, 0x65, 0x6C, 0x00, 0x00};
static const GUID IID_IAIMPMLDataStorageCommandDeleteFiles2 = {0x41494D50, 0x4D4C, 0x4453, 0x43, 0x6D, 0x64, 0x44, 0x65, 0x6C, 0x32, 0x00};
static const GUID IID_IAIMPMLDataStorageCommandDropData = {0x41494D50, 0x4D4C, 0x4453, 0x43, 0x6D, 0x64, 0x44, 0x72, 0x6F, 0x70, 0x00};
static const GUID IID_IAIMPMLDataStorageCommandFindInLibrary = {0x41494D50, 0x4D4C, 0x4453, 0x43, 0x6D, 0x64, 0x46, 0x6E, 0x64, 0x49, 0x6E};
static const GUID IID_IAIMPMLDataStorageCommandReloadTags = {0x41494D50, 0x4D4C, 0x4453, 0x43, 0x6D, 0x64, 0x55, 0x70, 0x54, 0x61, 0x67};
static const GUID IID_IAIMPMLDataStorageCommandReportDialog = {0x41494D50, 0x4D4C, 0x4453, 0x43, 0x6D, 0x64, 0x52, 0x70, 0x72, 0x74, 0x00};
static const GUID IID_IAIMPMLDataStorageCommandUserMark = {0x41494D50, 0x4D4C, 0x4453, 0x43, 0x6D, 0x64, 0x4D, 0x61, 0x72, 0x6B, 0x00};
static const GUID IID_IAIMPMLDataStorageManager = {0x41494D50, 0x4D4C, 0x4453, 0x4D, 0x6E, 0x67, 0x72, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPMLExtensionDataStorage = {0x41494D50, 0x4578, 0x744D, 0x4C, 0x44, 0x53, 0x00, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPMLFileList = {0x41494D50, 0x4D4C, 0x4669, 0x6C, 0x65, 0x4C, 0x69, 0x73, 0x74, 0x00, 0x00};
static const GUID IID_IAIMPMLGroupingPreset = {0x41494D50, 0x4D4C, 0x4772, 0x70, 0x50, 0x73, 0x74, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPMLGroupingPresets = {0x41494D50, 0x4D4C, 0x4772, 0x50, 0x72, 0x73, 0x74, 0x73, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPMLGroupingPresetStandard = {0x41494D50, 0x4D4C, 0x4772, 0x70, 0x50, 0x73, 0x74, 0x53, 0x74, 0x64, 0x00};
static const GUID IID_IAIMPMLGroupingTreeDataProvider = {0x41494D50, 0x4D4C, 0x4772, 0x70, 0x44, 0x61, 0x74, 0x61, 0x50, 0x72, 0x76};
static const GUID IID_IAIMPMLGroupingTreeDataProvider2 = {0x41494D50, 0x4D4C, 0x4772, 0x70, 0x44, 0x61, 0x74, 0x61, 0x50, 0x72, 0x32};
static const GUID IID_IAIMPMLGroupingTreeDataProviderSelection = {0x41494D50, 0x4D4C, 0x4772, 0x44, 0x74, 0x50, 0x72, 0x76, 0x53, 0x65, 0x6C};
static const GUID IID_IAIMPMLGroupingTreeSelection = {0x41494D50, 0x4D4C, 0x4770, 0x54, 0x72, 0x65, 0x65, 0x53, 0x65, 0x6C, 0x00};
static const GUID IID_IAIMPMLPlaylistPreimage = {0x414D4C53, 0x6D50, 0x6C73, 0x53, 0x72, 0x63, 0x00, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPMLSortDirection = {0x41494D50, 0x4D4C, 0x536F, 0x72, 0x74, 0x44, 0x69, 0x72, 0x74, 0x6E, 0x00};
static const GUID IID_IAIMPServiceMusicLibrary = {0x41494D50, 0x5372, 0x764D, 0x4C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

// Property ID for IAIMPPropertyList of IAIMPMLExtensionDataStorage
const int AIMPML_DATASTORAGE_PROPID_ID              = 0;
const int AIMPML_DATASTORAGE_PROPID_CAPTION         = 1;
const int AIMPML_DATASTORAGE_PROPID_CAPABILITIES    = 2;
const int AIMPML_DATASTORAGE_PROPID_GROUPINGPRESET  = 20;

// List of known Capabilities for AIMPML_DATASTORAGE_PROPID_CAPABILITIES
const int AIMPML_DATASTORAGE_CAP_FILTERING       = 1; // return it, if plugin has own implementation of data filtering
const int AIMPML_DATASTORAGE_CAP_PREIMAGES       = 2;
const int AIMPML_DATASTORAGE_CAP_GROUPINGPRESETS = 4;
const int AIMPML_DATASTORAGE_CAP_CUSTOMIZEGROUPS = 8;
const int AIMPML_DATASTORAGE_CAP_AUTOEXPANDFILES = 16;
const int AIMPML_DATASTORAGE_CAP_NOBOOKMARKS	 = 32;

// Schema Flags for IAIMPMLExtensionDataStorage.GetFields
const int AIMPML_FIELDS_SCHEMA_ALL                        = 0;
const int AIMPML_FIELDS_SCHEMA_TABLE_GROUPBY              = 2;
const int AIMPML_FIELDS_SCHEMA_TABLE_GROUPDETAILS         = 3;
const int AIMPML_FIELDS_SCHEMA_TABLE_VIEW_DEFAULT         = 10;
const int AIMPML_FIELDS_SCHEMA_TABLE_VIEW_GROUPDETAILS    = 11;
const int AIMPML_FIELDS_SCHEMA_TABLE_VIEW_ALBUMTHUMBNAILS = 12;

// Schema Flags for IAIMPMLExtensionDataStorage.GetGroupingPresets
const int AIMPML_GROUPINGPRESETS_SCHEMA_BUILTIN           = 1;
const int AIMPML_GROUPINGPRESETS_SCHEMA_DEFAULT           = 2;

// Property ID for IAIMPMLDataField
const int AIMPML_FIELD_PROPID_CUSTOM   = 0;
const int AIMPML_FIELD_PROPID_NAME     = 1;
const int AIMPML_FIELD_PROPID_TYPE     = 2;
const int AIMPML_FIELD_PROPID_FLAGS    = 3;
const int AIMPML_FIELD_PROPID_IMAGE    = 4;
const int AIMPML_FIELD_PROPID_DISPLAYVALUE = 5;

// ImageIndexes for AIMPML_FIELD_PROPID_IMAGE
const int AIMPML_FIELDIMAGE_FOLDER   = 0;
const int AIMPML_FIELDIMAGE_ARTIST   = 1;
const int AIMPML_FIELDIMAGE_DISK     = 2;
const int AIMPML_FIELDIMAGE_NOTE     = 3;
const int AIMPML_FIELDIMAGE_STAR     = 4;
const int AIMPML_FIELDIMAGE_CALENDAR = 5;
const int AIMPML_FIELDIMAGE_LABEL    = 6;

// Field Types
const int AIMPML_FIELDTYPE_INT32       = 1;
const int AIMPML_FIELDTYPE_INT64       = 2;
const int AIMPML_FIELDTYPE_FLOAT       = 3;
const int AIMPML_FIELDTYPE_STRING      = 4;
const int AIMPML_FIELDTYPE_STRINGSET   = 5;
const int AIMPML_FIELDTYPE_DATETIME    = 10;
const int AIMPML_FIELDTYPE_DURATION    = 11;
const int AIMPML_FIELDTYPE_FILESIZE    = 12;
const int AIMPML_FIELDTYPE_FILENAME    = 13;

// Field Flags
const int AIMPML_FIELDFLAG_GROUPING    = 1;
const int AIMPML_FIELDFLAG_FILTERING   = 2;
const int AIMPML_FIELDFLAG_INTERNAL    = 4;
const int AIMPML_FIELDFLAG_REQUIRED    = 8;

// Built-in Reserved Field Names
static const WCHAR* AIMPML_RESERVED_FIELD_ID       = L"ID";       // !REQUIRED! unique record id (Int32, Int64 or String)
static const WCHAR* AIMPML_RESERVED_FIELD_FILENAME = L"FileName"; // !REQUIRED! string
static const WCHAR* AIMPML_RESERVED_FIELD_FILESIZE = L"FileSize"; // Int64, in bytes
static const WCHAR* AIMPML_RESERVED_FIELD_DURATION = L"Duration"; // double, in seconds
static const WCHAR* AIMPML_RESERVED_FIELD_USERMARK = L"UserMark"; // integer, 0.0 .. 5.0

// Property ID for IAIMPMLGroupingPreset
const int AIMPML_GROUPINGPRESET_PROPID_CUSTOM   = 0;
const int AIMPML_GROUPINGPRESET_PROPID_ID	= 1;
const int AIMPML_GROUPINGPRESET_PROPID_NAME     = 2;

// Property ID for IAIMPMLGroupingPresetStandard
const int AIMPML_GROUPINGPRESETSTD_PROPID_FIELDS   = 10;

// Property ID for IAIMPMLDataFieldFilter
const int AIMPML_FIELDFILTER_FIELD     = 1;
const int AIMPML_FIELDFILTER_OPERATION = 2; // Refer to the AIMPML_FIELDFILTER_OPERATION_XXX
const int AIMPML_FIELDFILTER_VALUE1    = 3;
const int AIMPML_FIELDFILTER_VALUE2    = 4;

// FieldFilter Operations
const int AIMPML_FIELDFILTER_OPERATION_EQUALS = 0;
const int AIMPML_FIELDFILTER_OPERATION_NOTEQUALS = 1;
const int AIMPML_FIELDFILTER_OPERATION_BETWEEN = 2;
const int AIMPML_FIELDFILTER_OPERATION_LESSTHAN = 3;
const int AIMPML_FIELDFILTER_OPERATION_LESSTHANOREQUALS = 4;
const int AIMPML_FIELDFILTER_OPERATION_GREATERTHAN = 5;
const int AIMPML_FIELDFILTER_OPERATION_GREATERTHANOREQUALS = 6;
const int AIMPML_FIELDFILTER_OPERATION_CONTAINS = 7;
const int AIMPML_FIELDFILTER_OPERATION_BEGINSWITH = 8;
const int AIMPML_FIELDFILTER_OPERATION_ENDSWITH = 9;
const int AIMPML_FIELDFILTER_OPERATION_ISLASTXDAYS = 10;

// Property ID for IAIMPMLDataFilterGroup
const int AIMPML_FILTERGROUP_OPERATION = 1; // Refer to the AIMPML_FILTERGROUP_OPERATION_XXX

// FilterGroup Operations
const int AIMPML_FILTERGROUP_OPERATION_OR = 0;
const int AIMPML_FILTERGROUP_OPERATION_AND = 1;
const int AIMPML_FILTERGROUP_OPERATION_NOTOR = 2;
const int AIMPML_FILTERGROUP_OPERATION_NOTAND = 3;

// Property ID for IAIMPMLDataFieldFilterByArray
const int AIMPML_FIELDFILTERBYARRAY_FIELD = 1;

// Property ID for IAIMPMLDataFilter
const int AIMPML_FILTER_LIMIT           = 11;
const int AIMPML_FILTER_OFFSET          = 12;
const int AIMPML_FILTER_SORTBYLIST      = 15;
const int AIMPML_FILTER_SEARCHSTRING    = 20; // optional
const int AIMPML_FILTER_ALPHABETICINDEX = 21; // optional

// Sort Direction
const int AIMPML_SORTDIRECTION_UNDEFINED  = 0;
const int AIMPML_SORTDIRECTION_ASCENDING  = 1;
const int AIMPML_SORTDIRECTION_DESCENDING = 2;

// Flags for IAIMPMLGroupingTreeDataProvider.GetCapabilities
const int AIMPML_GROUPINGTREEDATAPROVIDER_CAP_HIDEALLDATA = 1;
const int AIMPML_GROUPINGTREEDATAPROVIDER_CAP_DONTSORT    = 2;

// Flags for IAIMPMLGroupingTreeDataProviderSelection.GetFlags
const int AIMPML_GROUPINGTREENODE_FLAG_HASCHILDREN = 1;
const int AIMPML_GROUPINGTREENODE_FLAG_STANDALONE  = 2;

// Flags for IAIMPServiceMusicLibraryUI.GetFiles
const int AIMPML_GETFILES_FLAGS_ALL      = 0;
const int AIMPML_GETFILES_FLAGS_SELECTED = 1;
const int AIMPML_GETFILES_FLAGS_FOCUSED  = 2;

// LocalDataStorage
static const WCHAR* AIMPML_LOCALDATASTORAGE_ID = L"TAIMPMLLocalDataStorage";

static const WCHAR* AIMPML_LOCALDATASTORAGE_FIELD_ID = L"ID"; // Int32
static const WCHAR* AIMPML_LOCALDATASTORAGE_FIELD_ADDDED = L"Added"; // DateTime (Float);
static const WCHAR* AIMPML_LOCALDATASTORAGE_FIELD_ALBUM = L"Album"; // String, multiple values
static const WCHAR* AIMPML_LOCALDATASTORAGE_FIELD_ALBUMARTIST = L"AlbumArtist"; // String, multiple values
static const WCHAR* AIMPML_LOCALDATASTORAGE_FIELD_ARTIST = L"Artist"; // String, multiple values
static const WCHAR* AIMPML_LOCALDATASTORAGE_FIELD_BITDEPTH = L"BitDepth"; // Int32;
static const WCHAR* AIMPML_LOCALDATASTORAGE_FIELD_BITRATE = L"Bitrate"; // Int32
static const WCHAR* AIMPML_LOCALDATASTORAGE_FIELD_BPM = L"BPM"; // Int32;
static const WCHAR* AIMPML_LOCALDATASTORAGE_FIELD_CHANNELS = L"Channels"; // Int32;
static const WCHAR* AIMPML_LOCALDATASTORAGE_FIELD_COMMENT = L"Comment"; // String (Memo)
static const WCHAR* AIMPML_LOCALDATASTORAGE_FIELD_COMPOSER = L"Composer"; // String, multiple values
static const WCHAR* AIMPML_LOCALDATASTORAGE_FIELD_CONDUCTOR = L"Conductor"; // String
static const WCHAR* AIMPML_LOCALDATASTORAGE_FIELD_COPYRIGHTS = L"Copyrights"; // String
static const WCHAR* AIMPML_LOCALDATASTORAGE_FIELD_DISKNUMBER = L"DiskNumber"; // String
static const WCHAR* AIMPML_LOCALDATASTORAGE_FIELD_DURATION = L"Duration"; // = AIMPML_RESERVED_FIELD_DURATION; // Duration (Float)
static const WCHAR* AIMPML_LOCALDATASTORAGE_FIELD_FILEFORMAT = L"FileFormat"; // String
static const WCHAR* AIMPML_LOCALDATASTORAGE_FIELD_FILENAME = L"FileName"; // = AIMPML_RESERVED_FIELD_FILENAME; // FileName (String);
static const WCHAR* AIMPML_LOCALDATASTORAGE_FIELD_FILESIZE =L"FileSize"; // = AIMPML_RESERVED_FIELD_FILESIZE; // FileSize (Int64);
static const WCHAR* AIMPML_LOCALDATASTORAGE_FIELD_GENRE = L"Genre"; // String, multiple values
static const WCHAR* AIMPML_LOCALDATASTORAGE_FIELD_LABELS = L"Labels"; // String, multiple values
static const WCHAR* AIMPML_LOCALDATASTORAGE_FIELD_LASTMODIFICATION = L"LastModification"; // DateTime (Float)
static const WCHAR* AIMPML_LOCALDATASTORAGE_FIELD_LASTPLAYBACK = L"LastPlayback"; // DateTime (Float);
static const WCHAR* AIMPML_LOCALDATASTORAGE_FIELD_LYRICIST = L"Lyricist"; // String, multiple values
static const WCHAR* AIMPML_LOCALDATASTORAGE_FIELD_MOOD = L"Mood"; // String, multiple values
static const WCHAR* AIMPML_LOCALDATASTORAGE_FIELD_PLAYBACKCOUNT = L"PlaybackCount"; // Int32
static const WCHAR* AIMPML_LOCALDATASTORAGE_FIELD_PUBLISHER = L"Publisher"; // String, multiple values
static const WCHAR* AIMPML_LOCALDATASTORAGE_FIELD_RATING = L"Rating"; // Int32
static const WCHAR* AIMPML_LOCALDATASTORAGE_FIELD_SAMPLERATE = L"SampleRate"; // Int32
static const WCHAR* AIMPML_LOCALDATASTORAGE_FIELD_TITLE = L"Title"; // String
static const WCHAR* AIMPML_LOCALDATASTORAGE_FIELD_TRACKNUMBER = L"TrackNumber"; // String
static const WCHAR* AIMPML_LOCALDATASTORAGE_FIELD_URL = L"URL"; // String
static const WCHAR* AIMPML_LOCALDATASTORAGE_FIELD_USERMARK = L"UserMark"; // = AIMPML_RESERVED_FIELD_USERMARK;
static const WCHAR* AIMPML_LOCALDATASTORAGE_FIELD_YEAR = L"Year"; // String


//----------------------------------------------------------------------------------------------------------------------
// Common Classes
//----------------------------------------------------------------------------------------------------------------------

/* IAIMPMLDataField */

class IAIMPMLDataField : public IAIMPPropertyList
{
};

/* IAIMPMLDataFieldDisplayValue */

class IAIMPMLDataFieldDisplayValue : public IUnknown
{
	public:
		virtual WCHAR* WINAPI GetDisplayValue(VARIANT* Value, int* Length) = 0;
};

/* IAIMPMLDataFieldFilter */

class IAIMPMLDataFieldFilter : public IAIMPPropertyList2
{
};

/* IAIMPMLDataFieldFilterByArray */

class IAIMPMLDataFieldFilterByArray : public IAIMPPropertyList2
{
	public:
		virtual HRESULT WINAPI GetData(VARIANT* Values, int* Count) = 0;
		virtual HRESULT WINAPI SetData(VARIANT* Values, int Count) = 0;
};

/* IAIMPMLDataFilterGroup */

class IAIMPMLDataFilterGroup : public IAIMPPropertyList2
{
	public:
		virtual HRESULT WINAPI Add(IUnknown* Field, VARIANT* Value1, VARIANT* Value2, int Operation, IAIMPMLDataFieldFilter** Filter) = 0;
		virtual HRESULT WINAPI Add2(IUnknown* Field, VARIANT* Values, int Count, IAIMPMLDataFieldFilterByArray** Filter) = 0;
		virtual HRESULT WINAPI AddGroup(IAIMPMLDataFilterGroup** Group) = 0;
		virtual HRESULT WINAPI Clear() = 0;
		virtual HRESULT WINAPI Delete(int Index) = 0;
		virtual HRESULT WINAPI GetChild(int Index, REFIID IID, void **Obj) = 0;
		virtual int WINAPI GetChildCount() = 0;
};

/* IAIMPMLDataFilter */

class IAIMPMLDataFilter : public IAIMPMLDataFilterGroup
{
	public:
		virtual HRESULT WINAPI Assign(IAIMPMLDataFilter* Source) = 0;
		virtual HRESULT WINAPI Clone(void **Filter) = 0;
};

/* IAIMPMLFileList */

class IAIMPMLFileList : public IUnknown
{
	public:
		virtual HRESULT WINAPI Add(VARIANT* ID, IAIMPString* FileName) = 0;
		virtual HRESULT WINAPI Clear() = 0;
		virtual HRESULT WINAPI Delete(int Index) = 0;
		virtual HRESULT WINAPI Insert(int Index, VARIANT* ID, IAIMPString* FileName) = 0;

		virtual int WINAPI GetCount() = 0;
		virtual HRESULT WINAPI GetFileName(int Index, IAIMPString** FileName) = 0;
		virtual HRESULT WINAPI SetFileName(int Index, IAIMPString* FileName) = 0;
		virtual HRESULT WINAPI GetID(int Index, VARIANT* ID) = 0;
		virtual HRESULT WINAPI SetID(int Index, VARIANT* ID) = 0;

		virtual HRESULT WINAPI Clone(void** Obj) = 0;
};

/* IAIMPMLSortDirection */

class IAIMPMLSortDirection: public IUnknown
{
	public:
		virtual HRESULT WINAPI GetValue(int* Value) = 0;
		virtual HRESULT WINAPI SetValue(int Value) = 0;
};

//----------------------------------------------------------------------------------------------------------------------
// Preimage
//----------------------------------------------------------------------------------------------------------------------

/* IAIMPMLPlaylistPreimage */

class IAIMPMLPlaylistPreimage : public IAIMPPlaylistPreimage
{
	public:
		virtual HRESULT WINAPI GetFilter(IAIMPMLDataFilter** Filter) = 0;
		virtual HRESULT WINAPI GetStorage(IUnknown** Storage) = 0;
};

//----------------------------------------------------------------------------------------------------------------------
// Data Providers
//----------------------------------------------------------------------------------------------------------------------

/* IAIMPMLAlbumArtProvider */

class IAIMPMLAlbumArtProvider : public IUnknown
{
	public:
		virtual HRESULT WINAPI Get(IAIMPObjectList* Fields, VARIANT* Values, IAIMPPropertyList* Options, IAIMPImageContainer** Image) = 0;
};

/* IAIMPMLAlbumArtProvider2 */

class IAIMPMLAlbumArtProvider2 : public IUnknown
{
	public:
		virtual HRESULT WINAPI Get(IAIMPObjectList* Fields, VARIANT* Values, IAIMPAlbumArtRequest* Request, IAIMPImageContainer** Image) = 0;
};

/* IAIMPMLDataProvider */

class IAIMPMLDataProvider : public IUnknown
{
	public:
		virtual HRESULT WINAPI GetData(IAIMPObjectList* Fields, IAIMPMLDataFilter* Filter, IUnknown** Data) = 0;
};

/* IAIMPMLDataProvider2 */

class IAIMPMLDataProvider2 : public IUnknown
{
	public:
		virtual HRESULT WINAPI GetData(
					 IAIMPObjectList* Fields,
					 IAIMPMLDataFilter* Filter,
					 IUnknown* Reserved,
		 /*in /out*/ IUnknown** PageID,
					 IUnknown** Data) = 0;
};

/* IAIMPMLDataProviderSelection */

class IAIMPMLDataProviderSelection : public IUnknown
{
	public:
		virtual DOUBLE WINAPI GetValueAsFloat(int FieldIndex) = 0;
		virtual int WINAPI GetValueAsInt32(int FieldIndex) = 0;
		virtual INT64 WINAPI GetValueAsInt64(int FieldIndex) = 0;
		virtual WCHAR* WINAPI GetValueAsString(int FieldIndex, int* Length) = 0;
		virtual BOOL WINAPI NextRow() = 0;
		// Deprecated, use IAIMPMLDataProvider2 instead
		// virtual BOOL WINAPI HasNextPage() = 0;
};

/* IAIMPMLGroupingTreeSelection */

class IAIMPMLGroupingTreeSelection : public IUnknown
{
	public:
		virtual int WINAPI GetCount() = 0;
		virtual HRESULT WINAPI GetValue(int Index, IAIMPString** FieldName, VARIANT* Value) = 0;
};

/* IAIMPMLGroupingTreeDataProviderSelection */

class IAIMPMLGroupingTreeDataProviderSelection : public IUnknown
{
	public:
		virtual HRESULT WINAPI GetDisplayValue(IAIMPString** S) = 0;
		virtual DWORD WINAPI GetFlags() = 0;
		virtual HRESULT WINAPI GetImageIndex(int* Index) = 0;
		virtual HRESULT WINAPI GetValue(IAIMPString** FieldName, VARIANT* Value) = 0;
		virtual BOOL WINAPI NextRow() = 0;
};

/* IAIMPMLGroupingTreeDataProvider */

class IAIMPMLGroupingTreeDataProvider : public IUnknown
{
	public:
		virtual HRESULT WINAPI AppendFilter(IAIMPMLDataFilterGroup* Filter, IAIMPMLGroupingTreeSelection* Selection) = 0;
		virtual DWORD WINAPI GetCapabilities() = 0;
		virtual HRESULT WINAPI GetData(IAIMPMLGroupingTreeSelection* Selection, IAIMPMLGroupingTreeDataProviderSelection** Data) = 0;
		virtual HRESULT WINAPI GetFieldForAlphabeticIndex(IAIMPString** FieldName) = 0;
};

/* IAIMPMLGroupingTreeDataProvider2 */

class IAIMPMLGroupingTreeDataProvider2 : public IAIMPMLGroupingTreeDataProvider
{
	public:
		virtual WCHAR WINAPI GetPathSeparator() = 0;
};

//----------------------------------------------------------------------------------------------------------------------
// Storage
//----------------------------------------------------------------------------------------------------------------------

/* IAIMPMLGroupingPreset */

class IAIMPMLGroupingPreset : public IAIMPPropertyList
{
};

/* IAIMPMLGroupingPresetStandard */

class IAIMPMLGroupingPresetStandard: public IAIMPMLGroupingPreset
{
	public:
		virtual HRESULT WINAPI GetFilter(IAIMPMLDataFilterGroup** Filter) = 0;
};

/* IAIMPMLGroupingPresets */

class IAIMPMLGroupingPresets : public IUnknown
{
	public:
		virtual HRESULT WINAPI BeginUpdate() = 0;
		virtual HRESULT WINAPI EndUpdate() = 0;

		virtual HRESULT WINAPI Add(IAIMPString* ID, IAIMPString* Name, DWORD Reserved,
			IAIMPMLGroupingTreeDataProvider* Provider, IAIMPMLGroupingPreset** Preset) = 0;
		virtual HRESULT WINAPI Add2(IAIMPString* ID, IAIMPString* Name, DWORD Reserved,
			IAIMPObjectList* FieldNames, IAIMPMLGroupingPresetStandard** Preset) = 0;
		virtual HRESULT WINAPI Add3(IAIMPString* ID, IAIMPString* Name, DWORD Reserved,
			IAIMPString* FieldName, IAIMPMLGroupingPresetStandard** Preset) = 0;
		virtual HRESULT WINAPI Delete(int Index) = 0;
		virtual HRESULT WINAPI Get(int Index, REFIID IID, void **Obj) = 0;
		virtual HRESULT WINAPI GetByID(IAIMPString* ID, REFIID IID, void **Obj) = 0;
		virtual int WINAPI GetCount() = 0;
};

/* IAIMPMLDataStorageManager */

class IAIMPMLDataStorageManager : public IUnknown
{
	public:
		virtual void WINAPI BackgroundTaskStarted(int ID, IAIMPString* Caption, IAIMPActionEvent* CancelEvent) = 0;
		virtual void WINAPI BackgroundTaskFinished(int ID) = 0;
		virtual void WINAPI Changed() = 0;
};

/* IAIMPMLDataStorage */

class IAIMPMLDataStorage : public IAIMPPropertyList // + IAIMPMLGroupingPresets
{
};

/* IAIMPMLDataStorage2 */

class IAIMPMLDataStorage2 : public IAIMPMLDataStorage // + IAIMPMLDataProvider, IAIMPMLDataProvider2
{
	public:
		virtual HRESULT WINAPI CreateObject(REFIID IID, void **Obj) = 0;
};

//----------------------------------------------------------------------------------------------------------------------
// Commands
//----------------------------------------------------------------------------------------------------------------------

class IAIMPMLDataStorageCommandAddFiles : public IUnknown
{
	public:
		virtual HRESULT WINAPI Add(IAIMPObjectList* Files) = 0;
};

/* IAIMPMLDataStorageCommandAddFilesDialog */

class IAIMPMLDataStorageCommandAddFilesDialog : public IUnknown
{
	public:
		virtual HRESULT WINAPI Execute(HWND OwnerHandle) = 0;
};

/* IAIMPMLDataStorageCommandDeleteFiles */

class IAIMPMLDataStorageCommandDeleteFiles : public IUnknown
{
	public:
		virtual BOOL WINAPI CanDelete(BOOL Physically) = 0;
		virtual HRESULT WINAPI Delete(IAIMPMLFileList* Files, BOOL Physically) = 0;
};

/* IAIMPMLDataStorageCommandDeleteFiles2 */

class IAIMPMLDataStorageCommandDeleteFiles2 : public IAIMPMLDataStorageCommandDeleteFiles
{
	public:
		virtual HRESULT WINAPI Delete2(IAIMPMLDataFilter* Filter, BOOL Physically) = 0;
};

/* IAIMPMLDataStorageCommandDropData */

class IAIMPMLDataStorageCommandDropData : public IUnknown
{
	public:
		virtual HRESULT WINAPI DropData() = 0;
};

/* IAIMPMLDataStorageCommandFindInLibrary */

class IAIMPMLDataStorageCommandFindInLibrary : public IUnknown
{
	public:
		virtual HRESULT WINAPI Find(
			IAIMPFileInfo *FileInfo,
			IAIMPString** GroupingPresetID,
			IAIMPMLGroupingTreeSelection** GroupingTreeSelection) = 0;
};

/* IAIMPMLDataStorageCommandReloadTags */

class IAIMPMLDataStorageCommandReloadTags : public IUnknown
{
	public:
		virtual HRESULT WINAPI ReloadTags(IAIMPMLFileList* Files) = 0;
};

/* IAIMPMLDataStorageCommandReportDialog */

class IAIMPMLDataStorageCommandReportDialog : public IUnknown
{
	public:
		virtual HRESULT WINAPI Execute(HWND OwnerHandle) = 0;
};

/* IAIMPMLDataStorageCommandUserMark */

class IAIMPMLDataStorageCommandUserMark : public IUnknown
{
	public:
		virtual HRESULT WINAPI SetMark(VARIANT* ID, const DOUBLE Value) = 0;
};

//----------------------------------------------------------------------------------------------------------------------
// Extensions
//----------------------------------------------------------------------------------------------------------------------

/* IAIMPMLExtensionDataStorage */

class IAIMPMLExtensionDataStorage : public IAIMPPropertyList // + IAIMPMLDataProvider
{
	public:
		virtual void WINAPI Finalize() = 0;
		virtual void WINAPI Initialize(IAIMPMLDataStorageManager* Manager) = 0;
		// Config
		virtual HRESULT WINAPI ConfigLoad(IAIMPConfig *Config, IAIMPString* Section) = 0;
		virtual HRESULT WINAPI ConfigSave(IAIMPConfig *Config, IAIMPString* Section) = 0;
		// Schemas
		virtual HRESULT WINAPI GetFields(int Schema, IAIMPObjectList** List) = 0;
		virtual HRESULT WINAPI GetGroupingPresets(int Schema, IAIMPMLGroupingPresets* Presets) = 0;
		// Build-in Commands
		virtual void WINAPI FlushCache(int Reserved /*= 0*/) = 0;
};

//----------------------------------------------------------------------------------------------------------------------
// Services
//----------------------------------------------------------------------------------------------------------------------

/* IAIMPServiceMusicLibrary */

class IAIMPServiceMusicLibrary : public IUnknown
{
	public:
		virtual HRESULT WINAPI GetActiveStorage(REFIID IID, void **Obj) = 0;
		virtual HRESULT WINAPI SetActiveStorage(IUnknown* Storage) = 0;

		virtual HRESULT WINAPI GetStorage(int Index, REFIID IID, void **Obj) = 0;
		virtual HRESULT WINAPI GetStorageByID(IAIMPString* ID, REFIID IID, void **Obj) = 0;
		virtual int WINAPI GetStorageCount() = 0;
};

/* IAIMPServiceMusicLibraryUI */

class IAIMPServiceMusicLibraryUI : public IUnknown
{
	public:
		virtual HRESULT WINAPI GetFiles(DWORD Flags, IAIMPMLFileList* List) = 0;
		virtual HRESULT WINAPI GetGroupingFilter(IAIMPMLDataFilter** Filter) = 0;
		virtual HRESULT WINAPI GetGroupingFilterPath(IAIMPString** Path) = 0;
		virtual HRESULT WINAPI SetGroupingFilterPath(IAIMPString* Path) = 0;
};

#endif // !apiMusicLibraryH