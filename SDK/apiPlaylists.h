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

#ifndef apiPlaylistsH
#define apiPlaylistsH

#include <windows.h>
#include <unknwn.h>
#include "apiObjects.h"

static const GUID IID_IAIMPPlaylist = {0x41494D50, 0x506C, 0x7300, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPPlaylistGroup = {0x41494D50, 0x506C, 0x7347, 0x72, 0x6F, 0x75, 0x70, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPPlaylistItem = {0x41494D50, 0x506C, 0x7349, 0x74, 0x65, 0x6D, 0x00, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPPlaylistListener = {0x41494D50, 0x506C, 0x734C, 0x73, 0x74, 0x6E, 0x72, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPPlaylistQueue = {0x41494D50, 0x506C, 0x7351, 0x75, 0x65, 0x75, 0x65, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPExtensionPlaylistManagerListener = {0x41494D50, 0x4578, 0x7450, 0x6C, 0x73, 0x4D, 0x61, 0x6E, 0x4C, 0x74, 0x72};
static const GUID IID_IAIMPServicePlaylistManager = {0x41494D50, 0x5372, 0x7650, 0x6C, 0x73, 0x4D, 0x61, 0x6E, 0x00, 0x00, 0x00};

// Property IDs for IAIMPPlaylistItem
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
const int AIMP_PLAYLIST_ADD_FLAGS_NOASYNC       = 4;
const int AIMP_PLAYLIST_ADD_FLAGS_FILEINFO      = 8;

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

// IAIMPPlaylist.GetFiles:
const int AIMP_PLAYLIST_GETFILES_FLAGS_SELECTED_ONLY    = 0x1;
const int AIMP_PLAYLIST_GETFILES_FLAGS_VISIBLE_ONLY     = 0x2;
const int AIMP_PLAYLIST_GETFILES_FLAGS_COLLAPSE_VIRTUAL = 0x4;

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
		virtual HRESULT WINAPI Delete3(BOOL Physically, TAIMPPlaylistDeleteProc Proc, void* UserData) = 0;
		virtual HRESULT WINAPI DeleteAll() = 0;

		// Sorting
		virtual HRESULT WINAPI Sort(int Mode) = 0;
		virtual HRESULT WINAPI Sort2(IAIMPString* Template) = 0;
		virtual HRESULT WINAPI Sort3(TAIMPPlaylistCompareProc* Proc, void* UserData) = 0;

		// Locking
		virtual HRESULT WINAPI BeginUpdate() = 0;
		virtual HRESULT WINAPI EndUpdate() = 0;

		// Another Commands
		virtual HRESULT WINAPI Close(DWORD Flags) = 0;
		virtual HRESULT WINAPI GetFiles(DWORD Flags, IAIMPObjectList **List) = 0;
		virtual HRESULT WINAPI MergeGroup(IAIMPPlaylistGroup* Group) = 0;
		virtual HRESULT WINAPI ReloadFromPreimage() = 0;
		virtual HRESULT WINAPI ReloadInfo(BOOL Full) = 0;

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

/* IAIMPExtensionPlaylistManagerListener */

class IAIMPExtensionPlaylistManagerListener: public IUnknown
{
	public:
		virtual void WINAPI PlaylistActivated(IAIMPPlaylist* Playlist) = 0;
		virtual void WINAPI PlaylistAdded(IAIMPPlaylist* Playlist) = 0;
		virtual void WINAPI PlaylistRemoved(IAIMPPlaylist* Playlist) = 0;
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

#endif // !apiPlaylistsH