/************************************************/
/*                                              */
/*          AIMP Programming Interface          */
/*               v4.00 build 1660               */
/*                                              */
/*                Artem Izmaylov                */
/*                (C) 2006-2015                 */
/*                 www.aimp.ru                  */
/*                                              */
/*            Mail: support@aimp.ru             */
/*                                              */
/************************************************/

#ifndef apiPlaylistsH
#define apiPlaylistsH

#include <windows.h>
#include <unknwn.h>
#include "apiObjects.h"
#include "apiThreading.h"

static const GUID IID_IAIMPPlaylist = {0x41494D50, 0x506C, 0x7300, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPPlaylistGroup = {0x41494D50, 0x506C, 0x7347, 0x72, 0x6F, 0x75, 0x70, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPPlaylistItem = {0x41494D50, 0x506C, 0x7349, 0x74, 0x65, 0x6D, 0x00, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPPlaylistListener = {0x41494D50, 0x506C, 0x734C, 0x73, 0x74, 0x6E, 0x72, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPPlaylistListener2 = {0x41494D50, 0x506C, 0x734C, 0x73, 0x74, 0x6E, 0x72, 0x32, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPPlaylistQueue = {0x41494D50, 0x506C, 0x7351, 0x75, 0x65, 0x75, 0x65, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPPlaylistQueue2 = {0x41494D50, 0x506C, 0x7351, 0x75, 0x65, 0x75, 0x65, 0x32, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPPlaylistQueueListener = {0x41494D50, 0x506C, 0x7351, 0x75, 0x65, 0x75, 0x65, 0x4C, 0x73, 0x74, 0x00};
static const GUID IID_IAIMPExtensionPlaylistManagerListener = {0x41494D50, 0x4578, 0x7450, 0x6C, 0x73, 0x4D, 0x61, 0x6E, 0x4C, 0x74, 0x72};
static const GUID IID_IAIMPServicePlaylistManager = {0x41494D50, 0x5372, 0x7650, 0x6C, 0x73, 0x4D, 0x61, 0x6E, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPServicePlaylistManager2 = {0x41494D50, 0x536D, 0x504C, 0x4D, 0x6E, 0x67, 0x72, 0x32, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPPlaylistPreimageFolders = {0x41494D50, 0x536D, 0x504C, 0x53, 0x72, 0x63, 0x46, 0x6C, 0x64, 0x72, 0x73};
static const GUID IID_IAIMPPlaylistPreimageDataProvider = {0x41494D50, 0x536D, 0x506C, 0x73, 0x44, 0x61, 0x74, 0x61, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPPlaylistPreimageListener = {0x41494D50, 0x536D, 0x504C, 0x4D, 0x6E, 0x67, 0x72, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPPlaylistPreimage = {0x41494D50, 0x536D, 0x504C, 0x53, 0x72, 0x63, 0x00, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPExtensionPlaylistPreimageFactory = {0x41494D50, 0x4578, 0x7453, 0x6D, 0x50, 0x6C, 0x73, 0x46, 0x63, 0x74, 0x00};

// Property IDs for IAIMPPlaylistItem
const int AIMP_PLAYLISTITEM_PROPID_CUSTOM		  = 0;
const int AIMP_PLAYLISTITEM_PROPID_DISPLAYTEXT    = 1;
const int AIMP_PLAYLISTITEM_PROPID_FILEINFO       = 2;
const int AIMP_PLAYLISTITEM_PROPID_FILENAME       = 3;
const int AIMP_PLAYLISTITEM_PROPID_GROUP          = 4;
const int AIMP_PLAYLISTITEM_PROPID_INDEX          = 5;
const int AIMP_PLAYLISTITEM_PROPID_MARK           = 6;
const int AIMP_PLAYLISTITEM_PROPID_PLAYINGSWITCH  = 7;
const int AIMP_PLAYLISTITEM_PROPID_PLAYLIST       = 8;
const int AIMP_PLAYLISTITEM_PROPID_SELECTED       = 9;
const int AIMP_PLAYLISTITEM_PROPID_PLAYBACKQUEUEINDEX = 10;

// Property IDs for IAIMPPlaylistGroup
const int AIMP_PLAYLISTGROUP_PROPID_NAME      = 1;
const int AIMP_PLAYLISTGROUP_PROPID_EXPANDED  = 2;
const int AIMP_PLAYLISTGROUP_PROPID_DURATION  = 3;
const int AIMP_PLAYLISTGROUP_PROPID_INDEX     = 4;
const int AIMP_PLAYLISTGROUP_PROPID_SELECTED  = 5;

// Property IDs for IAIMPPropertyList from IAIMPPlaylistQueue
const int AIMP_PLAYLISTQUEUE_PROPID_SUSPENDED = 1;

// Property IDs for IAIMPPropertyList from IAIMPPlaylist
const int AIMP_PLAYLIST_PROPID_NAME                     = 1;
const int AIMP_PLAYLIST_PROPID_READONLY                 = 2;
const int AIMP_PLAYLIST_PROPID_FOCUSED_OBJECT           = 3;
const int AIMP_PLAYLIST_PROPID_ID                       = 4;
const int AIMP_PLAYLIST_PROPID_GROUPPING                = 10;
const int AIMP_PLAYLIST_PROPID_GROUPPING_OVERRIDEN      = 11;
const int AIMP_PLAYLIST_PROPID_GROUPPING_TEMPLATE       = 12;
const int AIMP_PLAYLIST_PROPID_GROUPPING_AUTOMERGING    = 13;
const int AIMP_PLAYLIST_PROPID_FORMATING_OVERRIDEN      = 20;
const int AIMP_PLAYLIST_PROPID_FORMATING_LINE1_TEMPLATE = 21;
const int AIMP_PLAYLIST_PROPID_FORMATING_LINE2_TEMPLATE = 22;
const int AIMP_PLAYLIST_PROPID_VIEW_OVERRIDEN           = 30;
const int AIMP_PLAYLIST_PROPID_VIEW_DURATION            = 31;
const int AIMP_PLAYLIST_PROPID_VIEW_EXPAND_BUTTONS      = 32;
const int AIMP_PLAYLIST_PROPID_VIEW_MARKS               = 33;
const int AIMP_PLAYLIST_PROPID_VIEW_NUMBERS             = 34;
const int AIMP_PLAYLIST_PROPID_VIEW_NUMBERS_ABSOLUTE    = 35;
const int AIMP_PLAYLIST_PROPID_VIEW_SECOND_LINE         = 36;
const int AIMP_PLAYLIST_PROPID_VIEW_SWITCHES            = 37;
const int AIMP_PLAYLIST_PROPID_FOCUSINDEX               = 50;
const int AIMP_PLAYLIST_PROPID_PLAYBACKCURSOR           = 51;
const int AIMP_PLAYLIST_PROPID_PLAYINGINDEX             = 52;
const int AIMP_PLAYLIST_PROPID_SIZE                     = 53;
const int AIMP_PLAYLIST_PROPID_DURATION                 = 54;
const int AIMP_PLAYLIST_PROPID_PREIMAGE                 = 60;

// Flags for IAIMPPlaylist.Add & IAIMPPlaylist.AddList
const int AIMP_PLAYLIST_ADD_FLAGS_NOCHECKFORMAT = 1;
const int AIMP_PLAYLIST_ADD_FLAGS_NOEXPAND      = 2;
const int AIMP_PLAYLIST_ADD_FLAGS_NOTHREADING   = 4;
const int AIMP_PLAYLIST_ADD_FLAGS_FILEINFO      = 8;

// Flags for IAIMPPlaylist.Delete3
const int AIMP_PLAYLIST_DELETE_FLAGS_PHYSICALLY     = 1;
const int AIMP_PLAYLIST_DELETE_FLAGS_NOCONFIRMATION = 2;

// Flags for IAIMPPlaylist.Sort
const int AIMP_PLAYLIST_SORTMODE_TITLE      = 1;
const int AIMP_PLAYLIST_SORTMODE_FILENAME   = 2;
const int AIMP_PLAYLIST_SORTMODE_DURATION   = 3;
const int AIMP_PLAYLIST_SORTMODE_ARTIST     = 4;
const int AIMP_PLAYLIST_SORTMODE_INVERSE    = 5;
const int AIMP_PLAYLIST_SORTMODE_RANDOMIZE  = 6;

// Flags for IAIMPPlaylist.Close
const int AIMP_PLAYLIST_CLOSE_FLAGS_FORCE_REMOVE = 1;
const int AIMP_PLAYLIST_CLOSE_FLAGS_FORCE_UNLOAD = 2;

// Flags for IAIMPPlaylist.GetFiles
const int AIMP_PLAYLIST_GETFILES_FLAGS_SELECTED_ONLY    = 0x1;
const int AIMP_PLAYLIST_GETFILES_FLAGS_VISIBLE_ONLY     = 0x2;
const int AIMP_PLAYLIST_GETFILES_FLAGS_COLLAPSE_VIRTUAL = 0x4;

// Flags for IAIMPPlaylist.ReloadInfo
const int AIMP_PLAYLIST_RELOADINFO_FLAGS_DEFAULT  = 0;
const int AIMP_PLAYLIST_RELOADINFO_FLAGS_FULL     = 1;
const int AIMP_PLAYLIST_RELOADINFO_FLAGS_SELECTED = 2;

// Flags for IAIMPPlaylistListener.Changed
const int AIMP_PLAYLIST_NOTIFY_NAME           = 1;
const int AIMP_PLAYLIST_NOTIFY_SELECTION      = 2;
const int AIMP_PLAYLIST_NOTIFY_PLAYBACKCURSOR = 4;
const int AIMP_PLAYLIST_NOTIFY_READONLY       = 8;
const int AIMP_PLAYLIST_NOTIFY_FOCUSINDEX     = 16;
const int AIMP_PLAYLIST_NOTIFY_CONTENT        = 32;
const int AIMP_PLAYLIST_NOTIFY_FILEINFO       = 64;
const int AIMP_PLAYLIST_NOTIFY_STATISTICS     = 128;
const int AIMP_PLAYLIST_NOTIFY_PLAYINGSWITCHS = 256;
const int AIMP_PLAYLIST_NOTIFY_PREIMAGE       = 512;

// Properties IDS for IAIMPPlaylistPreimage
const int AIMP_PLAYLISTPREIMAGE_PROPID_FACTORYID = 1;
const int AIMP_PLAYLISTPREIMAGE_PROPID_AUTOSYNC = 2;
const int AIMP_PLAYLISTPREIMAGE_PROPID_HASDIALOG = 3;
const int AIMP_PLAYLISTPREIMAGE_PROPID_SORTTEMPLATE = 4;

// Properties IDs for IAIMPPlaylistPreimageFolders
const int AIMP_PLAYLISTPREIMAGEFOLDERS_PROPID_AUTOSYNC_ON_STARTUP = 100;

// Built-in Preimage Factories
//const WCHAR* AIMP_PREIMAGEFACTORY_FOLDERS_ID = TEXT("TAIMPPlaylistFoldersPreimage");
//const WCHAR* AIMP_PREIMAGEFACTORY_MUSICLIBRARY_ID = TEXT("TAIMPMLPlaylistPreimage");

/* IAIMPPlaylistItem */

class IAIMPPlaylistItem: public IAIMPPropertyList
{
	public:
		virtual HRESULT WINAPI ReloadInfo() = 0;
};

/* IAIMPPlaylistGroup */

class IAIMPPlaylistGroup: public IAIMPPropertyList
{
	public:
		virtual HRESULT WINAPI GetItem(int Index, REFIID IID, void **Obj) = 0;
		virtual int WINAPI GetItemCount() = 0;
};

/* IAIMPPlaylistListener */

class IAIMPPlaylistListener: public IUnknown
{
	public:
		virtual void WINAPI Activated() = 0;
		virtual void WINAPI Changed(DWORD Flags) = 0;
		virtual void WINAPI Removed() = 0;
};

/* IAIMPPlaylistListener2 */

class IAIMPPlaylistListener2: public IUnknown
{
	public:
		virtual void WINAPI ScanningBegin() = 0;
		virtual void WINAPI ScanningProgress(const double Progress) = 0;
		virtual void WINAPI ScanningEnd(BOOL HasChanges, BOOL Canceled) = 0;
};

typedef int  (CALLBACK TAIMPPlaylistCompareProc)(IAIMPPlaylistItem* Item1, IAIMPPlaylistItem* Item2, void* UserData);
typedef BOOL (CALLBACK TAIMPPlaylistDeleteProc)(IAIMPPlaylistItem* Item, void* UserData);


/* IAIMPPlaylist */

class IAIMPPlaylist: public IUnknown
{
	public:
		// Adding
		virtual HRESULT WINAPI Add(IUnknown* Obj, DWORD Flags, int InsertIn) = 0;
		virtual HRESULT WINAPI AddList(IAIMPObjectList* ObjList, DWORD Flags, int InsertIn) = 0;

		// Deleting
		virtual HRESULT WINAPI Delete(IAIMPPlaylistItem* Item) = 0;
		virtual HRESULT WINAPI Delete2(int ItemIndex) = 0;
		virtual HRESULT WINAPI Delete3(DWORD Flags, TAIMPPlaylistDeleteProc Proc, void* UserData) = 0;
		virtual HRESULT WINAPI DeleteAll() = 0;

		// Sorting
		virtual HRESULT WINAPI Sort(int Mode) = 0;
		virtual HRESULT WINAPI Sort2(IAIMPString* Template) = 0;
		virtual HRESULT WINAPI Sort3(TAIMPPlaylistCompareProc* Proc, void* UserData) = 0;

		// Locking
		virtual HRESULT WINAPI BeginUpdate() = 0;
		virtual HRESULT WINAPI EndUpdate() = 0;

		// Other Commands
		virtual HRESULT WINAPI Close(DWORD Flags) = 0;
		virtual HRESULT WINAPI GetFiles(DWORD Flags, IAIMPObjectList **List) = 0;
		virtual HRESULT WINAPI MergeGroup(IAIMPPlaylistGroup* Group) = 0;
		virtual HRESULT WINAPI ReloadFromPreimage() = 0;
		virtual HRESULT WINAPI ReloadInfo(DWORD Flags) = 0;

		// Items
		virtual HRESULT WINAPI GetItem(int Index, REFIID IID, void **Obj) = 0;
		virtual int WINAPI GetItemCount() = 0;

		// Groups
		virtual HRESULT WINAPI GetGroup(int Index, REFIID IID, void **Obj) = 0;
		virtual int WINAPI GetGroupCount() = 0;

		// Listener
		virtual HRESULT WINAPI ListenerAdd(IAIMPPlaylistListener* AListener) = 0;
		virtual HRESULT WINAPI ListenerRemove(IAIMPPlaylistListener* AListener) = 0;
};

/* IAIMPPlaylistQueue */

class IAIMPPlaylistQueue: public IUnknown
{
	public:
		// Adding
		virtual HRESULT WINAPI Add(IAIMPPlaylistItem* Item, BOOL InsertAtBeginning) = 0;
		virtual HRESULT WINAPI AddList(IAIMPObjectList* ItemList, BOOL InsertAtBeginning) = 0;
		// Deleting
		virtual HRESULT WINAPI Delete(IAIMPPlaylistItem* Item) = 0;
		virtual HRESULT WINAPI Delete2(IAIMPPlaylist* Playlist) = 0;
		// Reorder
		virtual HRESULT WINAPI Move(IAIMPPlaylistItem* Item, int TargetIndex) = 0;
		virtual HRESULT WINAPI Move2(int ItemIndex, int TargetIndex) = 0;
		// Items
		virtual HRESULT WINAPI GetItem(int Index, REFIID IID, void **Obj) = 0;
		virtual int WINAPI GetItemCount() = 0;
};

/* IAIMPPlaylistQueueListener */

class IAIMPPlaylistQueueListener: public IUnknown
{
	public:
		virtual void WINAPI ContentChanged() = 0;
		virtual void WINAPI StateChanged() = 0;

};

/* IAIMPPlaylistQueue2 */

class IAIMPPlaylistQueue2: public IAIMPPlaylistQueue
{
	public:
		// Listener
		virtual HRESULT WINAPI ListenerAdd(IAIMPPlaylistQueueListener* AListener) = 0;
		virtual HRESULT WINAPI ListenerRemove(IAIMPPlaylistQueueListener* AListener) = 0;
};

/* IAIMPPlaylistPreimageListener */

class IAIMPPlaylistPreimageListener : public IUnknown
{
	public:
		virtual HRESULT WINAPI DataChanged() = 0;
		virtual HRESULT WINAPI SettingsChanged() = 0;
};

/* IAIMPPlaylistPreimage */

class IAIMPPlaylistPreimage: public IAIMPPropertyList
{
	public:
		virtual void WINAPI Finalize() = 0;
		virtual void WINAPI Initialize(IAIMPPlaylistPreimageListener* Listener) = 0;

		virtual HRESULT WINAPI ConfigLoad(IAIMPStream* Stream) = 0;
		virtual HRESULT WINAPI ConfigSave(IAIMPStream* Stream) = 0;
		virtual HRESULT WINAPI ExecuteDialog(HWND OwnerWndHanle) = 0;
};

/* IAIMPPlaylistPreimageDataProvider */

class IAIMPPlaylistPreimageDataProvider : public IUnknown 
{
	public:
		virtual HRESULT WINAPI GetFiles(IAIMPTaskOwner* Owner, DWORD** Flags, IAIMPObjectList** List) = 0;
};

/* IAIMPPlaylistPreimageFolders */

class IAIMPPlaylistPreimageFolders : public IAIMPPlaylistPreimage
{
	public:
		virtual HRESULT WINAPI ItemsAdd(IAIMPString* Path, BOOL Recursive) = 0;
		virtual HRESULT WINAPI ItemsDelete(int Index) = 0;
		virtual HRESULT WINAPI ItemsDeleteAll() = 0;
		virtual HRESULT WINAPI ItemsGet(int Index, IAIMPString* Path, BOOL* Recursive) = 0;
		virtual int WINAPI ItemsGetCount() = 0;
};

/* IAIMPExtensionPlaylistManagerListener */

class IAIMPExtensionPlaylistManagerListener: public IUnknown
{
	public:
		virtual void WINAPI PlaylistActivated(IAIMPPlaylist* Playlist) = 0;
		virtual void WINAPI PlaylistAdded(IAIMPPlaylist* Playlist) = 0;
		virtual void WINAPI PlaylistRemoved(IAIMPPlaylist* Playlist) = 0;
};

/* IAIMPExtensionPlaylistPreimageFactory */

class IAIMPExtensionPlaylistPreimageFactory : public IUnknown 
{
	public:
		virtual HRESULT WINAPI CreatePreimage(IAIMPPlaylistPreimage** preimage) = 0;
		virtual HRESULT WINAPI GetID(IAIMPString** ID) = 0;
		virtual HRESULT WINAPI GetName(IAIMPString** Name) = 0;
};


/* IAIMPServicePlaylistManager */

class IAIMPServicePlaylistManager: public IUnknown
{
	public:
		// Creating Playlist
    	virtual HRESULT WINAPI CreatePlaylist(IAIMPString* Name, BOOL Activate, IAIMPPlaylist **Playlist) = 0;
    	virtual HRESULT WINAPI CreatePlaylistFromFile(IAIMPString* FileName, BOOL Activate, IAIMPPlaylist **Playlist) = 0;

		// Active Playlist
    	virtual HRESULT WINAPI GetActivePlaylist(IAIMPPlaylist **Playlist) = 0;
    	virtual HRESULT WINAPI SetActivePlaylist(IAIMPPlaylist* Playlist) = 0;

		// Playable Playlist
    	virtual HRESULT WINAPI GetPlayablePlaylist(IAIMPPlaylist **Playlist) = 0;

		// Loaded Playlists
		virtual HRESULT WINAPI GetLoadedPlaylist(int Index, IAIMPPlaylist** Playlist) = 0;
		virtual HRESULT WINAPI GetLoadedPlaylistByName(IAIMPString* Name, IAIMPPlaylist** Playlist) = 0;
		virtual int WINAPI GetLoadedPlaylistCount() = 0;
		virtual HRESULT WINAPI GetLoadedPlaylistByID(IAIMPString* ID, IAIMPPlaylist** Playlist) = 0;
};

/* IAIMPServicePlaylistManager2 */

class IAIMPServicePlaylistManager2 : public IAIMPServicePlaylistManager 
{
	public:
		virtual HRESULT WINAPI GetPreimageFactory(int Index, IAIMPExtensionPlaylistPreimageFactory** Factory) = 0;
		virtual HRESULT WINAPI GetPreimageFactoryByID(IAIMPString* ID, IAIMPExtensionPlaylistPreimageFactory** Factory) = 0;
		virtual int WINAPI GetPreimageFactoryCount() = 0;
};

#endif // !apiPlaylistsH