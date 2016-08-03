/************************************************/
/*                                              */
/*          AIMP Programming Interface          */
/*               v4.00 build 1690               */
/*                                              */
/*                Artem Izmaylov                */
/*                (C) 2006-2016                 */
/*                 www.aimp.ru                  */
/*                                              */
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
static const GUID IID_IAIMPEqualizerPreset = {0x41494D50, 0x4571, 0x5072, 0x73, 0x74, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPPlaybackQueueItem = {0x41494D50, 0x506C, 0x6179, 0x62, 0x61, 0x63, 0x6B, 0x51, 0x49, 0x74, 0x6D};
static const GUID IID_IAIMPServicePlaybackQueue = {0x41494D50, 0x5372, 0x7650, 0x6C, 0x62, 0x61, 0x63, 0x6B, 0x51, 0x00, 0x00};
static const GUID IID_IAIMPServicePlayer = {0x41494D50, 0x5372, 0x7650, 0x6C, 0x61, 0x79, 0x65, 0x72, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPServicePlayerEqualizerPresets = {0x41494D50, 0x5372, 0x7645, 0x51, 0x50, 0x72, 0x73, 0x74, 0x73, 0x00, 0x00};
static const GUID IID_IAIMPServicePlayerEqualizer = {0x41494D50, 0x5372, 0x7645, 0x51, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPServiceWaveform = {0x41494D50, 0x5372, 0x7657, 0x61, 0x76, 0x65, 0x00, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPExtensionWaveformProvider = {0x41494D50, 0x4578, 0x7457, 0x61, 0x76, 0x50, 0x72, 0x76, 0x00, 0x00, 0x00};


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

// PropIDs for IAIMPPropertyList from IAIMPServicePlayer
const int AIMP_PLAYER_PROPID_STOP_AFTER_TRACK                     = 1;
const int AIMP_PLAYER_PROPID_AUTO_JUMP_TO_NEXT_TRACK              = 2;
const int AIMP_PLAYER_PROPID_AUTOSWITCHING                        = 10;
const int AIMP_PLAYER_PROPID_AUTOSWITCHING_CROSSFADE              = 11; // msec
const int AIMP_PLAYER_PROPID_AUTOSWITCHING_FADEIN                 = 12; // msec
const int AIMP_PLAYER_PROPID_AUTOSWITCHING_FADEOUT                = 13; // msec
const int AIMP_PLAYER_PROPID_AUTOSWITCHING_PAUSE_BETWEEN_TRACKS   = 14; // msec
const int AIMP_PLAYER_PROPID_MANUALSWITCHING                      = 20;
const int AIMP_PLAYER_PROPID_MANUALSWITCHING_CROSSFADE            = 21; // msec
const int AIMP_PLAYER_PROPID_MANUALSWITCHING_FADEIN               = 22; // msec
const int AIMP_PLAYER_PROPID_MANUALSWITCHING_FADEOUT              = 23; // msec

#pragma pack(push, 1)
struct TAIMPWaveformPeakInfo
{
	WORD MaxNegative;
	WORD MaxPositive;
};
#pragma pack(pop)
typedef TAIMPWaveformPeakInfo* PAIMPWaveformPeakInfo;


/* IAIMPEqualizerPreset */

class IAIMPEqualizerPreset: public IUnknown
{
	public:
		virtual HRESULT WINAPI GetName(IAIMPString **S) = 0;
		virtual HRESULT WINAPI SetName(IAIMPString *S) = 0;
		virtual HRESULT WINAPI GetBandValue(int BandIndex, double *S) = 0;
		virtual HRESULT WINAPI SetBandValue(int BandIndex, const double S) = 0;
};

/* IAIMPPlaybackQueueItem */

class IAIMPPlaybackQueueItem: public IAIMPPropertyList
{
};

/* IAIMPExtensionPlayerHook */

class IAIMPExtensionPlayerHook: public IUnknown
{
	public:
		virtual HRESULT WINAPI OnCheckURL(IAIMPString* URL, BOOL *Handled) = 0;
};

/* IAIMPExtensionPlaybackQueue */

class IAIMPExtensionPlaybackQueue: public IUnknown
{
	public:
		virtual HRESULT WINAPI GetNext(IUnknown* Current, DWORD Flags, IAIMPPlaybackQueueItem* QueueItem) = 0;
		virtual HRESULT WINAPI GetPrev(IUnknown* Current, DWORD Flags, IAIMPPlaybackQueueItem* QueueItem) = 0;
		virtual void WINAPI OnSelect(IAIMPPlaylistItem* Item, IAIMPPlaybackQueueItem* QueueItem) = 0;
};

/* IAIMPExtensionWaveformProvider */

class IAIMPExtensionWaveformProvider : public IUnknown 
{
	public:
		virtual HRESULT WINAPI Calculate(IAIMPString* FileURI, IAIMPTaskOwner* TaskOwner, PAIMPWaveformPeakInfo Peaks, int PeakCount) = 0;
};

/* IAIMPServicePlayer */

class IAIMPServicePlayer: public IUnknown // + IAIMPPropertyList
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

/* IAIMPServicePlayerEqualizer */

class IAIMPServicePlayerEqualizer: public IUnknown
{
	public:
		virtual BOOL WINAPI GetActive() = 0;
		virtual HRESULT WINAPI SetActive(BOOL Value) = 0;

		virtual HRESULT WINAPI GetBandValue(int BandIndex, double *Value) = 0;
		virtual HRESULT WINAPI SetBandValue(int BandIndex, const double Value) = 0;

		virtual HRESULT WINAPI GetPreset(IAIMPEqualizerPreset **Preset) = 0;
		virtual HRESULT WINAPI SetPreset(IAIMPEqualizerPreset *Preset) = 0;
};

/* IAIMPServicePlayerEqualizerPresets */

class IAIMPServicePlayerEqualizerPresets: public IUnknown
{
	public:
		virtual HRESULT WINAPI Add(IAIMPString* Name, IAIMPEqualizerPreset **Preset) = 0;
		virtual HRESULT WINAPI FindByName(IAIMPString* Name, IAIMPEqualizerPreset **Preset) = 0;
		virtual HRESULT WINAPI Delete(IAIMPEqualizerPreset *Preset) = 0;
		virtual HRESULT WINAPI Delete2(int Index) = 0;

		virtual HRESULT WINAPI GetPreset(int Index, IAIMPEqualizerPreset **Preset) = 0;
		virtual int WINAPI GetPresetCount() = 0;
};

/* IAIMPServicePlaybackQueue */

class IAIMPServicePlaybackQueue: public IUnknown
{
	public:
		virtual HRESULT WINAPI GetNextTrack(IAIMPPlaybackQueueItem **Item) = 0;
		virtual HRESULT WINAPI GetPrevTrack(IAIMPPlaybackQueueItem **Item) = 0;
};

/* IAIMPServiceWaveform */

class IAIMPServiceWaveform : public IUnknown
{
};

#endif // !apiPlayerH