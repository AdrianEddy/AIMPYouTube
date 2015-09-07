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

#ifndef apiPlayerH
#define apiPlayerH

#include <windows.h>
#include <unknwn.h>
#include "apiObjects.h"
#include "apiPlaylists.h"
#include "apiFileManager.h"

static const GUID IID_IAIMPExtensionPlaybackQueue = {0x41494D50, 0x4578, 0x7450, 0x6C, 0x61, 0x79, 0x62, 0x61, 0x63, 0x6B, 0x51};
static const GUID IID_IAIMPExtensionPlayerHook = {0x41494D50, 0x4578, 0x7450, 0x6C, 0x72, 0x48, 0x6F, 0x6F, 0x6B, 0x00, 0x00};
static const GUID IID_IAIMPPlaybackQueueItem = {0x41494D50, 0x506C, 0x6179, 0x62, 0x61, 0x63, 0x6B, 0x51, 0x49, 0x74, 0x6D};
static const GUID IID_IAIMPServicePlaybackQueue = {0x41494D50, 0x5372, 0x7650, 0x6C, 0x62, 0x61, 0x63, 0x6B, 0x51, 0x00, 0x00};
static const GUID IID_IAIMPServicePlayer = {0x41494D50, 0x5372, 0x7650, 0x6C, 0x61, 0x79, 0x65, 0x72, 0x00, 0x00, 0x00};

// PropIDs for IAIMPPlaybackQueueItem
const int AIMP_PLAYBACKQUEUEITEM_PROPID_CUSTOM        = 0;
const int AIMP_PLAYBACKQUEUEITEM_PROPID_PLAYLISTITEM  = 1;

// Flags for IAIMPExtensionPlaybackQueue.GetNext / GetPrev
const int AIMP_PLAYBACKQUEUE_FLAGS_START_FROM_BEGINNING = 1;
const int AIMP_PLAYBACKQUEUE_FLAGS_START_FROM_CURSOR    = 2;
const int AIMP_PLAYBACKQUEUE_FLAGS_START_FROM_ITEM      = 3;

// Flags for IAIMPServicePlayer.Play4
const int AIMP_SERVICE_PLAYER_FLAGS_PLAY_FROM_PLAYLIST              = 1;
const int AIMP_SERVICE_PLAYER_FLAGS_PLAY_FROM_PLAYLIST_CAN_ADD      = 2;
const int AIMP_SERVICE_PLAYER_FLAGS_PLAY_WITHOUT_ADDING_TO_PLAYLIST = 4;

/* IAIMPPlaybackQueueItem */

class IAIMPPlaybackQueueItem: public IAIMPPropertyList
{
};

/* IAIMPExtensionPlayerHook */

class IAIMPExtensionPlayerHook: public IUnknown
{
	public:
		virtual HRESULT WINAPI OnCheckURL(IAIMPString* URL, BOOL **Handled) = 0;
};

/* IAIMPExtensionPlaybackQueue */

class IAIMPExtensionPlaybackQueue: public IUnknown
{
	public:
		virtual HRESULT WINAPI GetNext(IUnknown* Current, DWORD Flags, IAIMPPlaybackQueueItem* QueueItem) = 0;
		virtual HRESULT WINAPI GetPrev(IUnknown* Current, DWORD Flags, IAIMPPlaybackQueueItem* QueueItem) = 0;
		virtual void WINAPI OnSelect(IAIMPPlaylistItem* Item, IAIMPPlaybackQueueItem* QueueItem) = 0;
};

/* IAIMPServicePlayer */

class IAIMPServicePlayer: public IUnknown
{
	public:
		// Start Playback
		virtual HRESULT WINAPI Play(IAIMPPlaybackQueueItem* Item) = 0;
		virtual HRESULT WINAPI Play2(IAIMPPlaylistItem* Item) = 0;
		virtual HRESULT WINAPI Play3(IAIMPPlaylist* Playlist) = 0;
		virtual HRESULT WINAPI Play4(IAIMPString* FileURI, DWORD Flags) = 0;
		// Navigation
		virtual HRESULT WINAPI GoToNext() = 0;
		virtual HRESULT WINAPI GoToPrev() = 0;
		// Playable File Control
		virtual HRESULT WINAPI GetDuration(double* Seconds) = 0;
		virtual HRESULT WINAPI GetPosition(double* Seconds) = 0;
		virtual HRESULT WINAPI SetPosition(const double Seconds) = 0;
		virtual HRESULT WINAPI GetMute(bool *Value) = 0;
		virtual HRESULT WINAPI SetMute(const bool Value) = 0;
		virtual HRESULT WINAPI GetVolume(float *Level) = 0;
		virtual HRESULT WINAPI SetVolume(const float Level) = 0;
		virtual HRESULT WINAPI GetInfo(IAIMPFileInfo** FileInfo) = 0;
		virtual HRESULT WINAPI GetPlaylistItem(IAIMPPlaylistItem **Item) = 0;
		virtual int WINAPI GetState() = 0;
		virtual HRESULT WINAPI Pause() = 0;
		virtual HRESULT WINAPI Resume() = 0;
		virtual HRESULT WINAPI Stop() = 0;
		virtual HRESULT WINAPI StopAfterTrack() = 0;
};

/* IAIMPServicePlaybackQueue */

class IAIMPServicePlaybackQueue: public IUnknown
{
	public:
		virtual HRESULT WINAPI GetNextTrack(IAIMPPlaybackQueueItem **Item) = 0;
		virtual HRESULT WINAPI GetPrevTrack(IAIMPPlaybackQueueItem **Item) = 0;
};

#endif // !apiPlayerH