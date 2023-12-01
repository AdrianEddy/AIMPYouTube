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

#ifndef apiDecodersH
#define apiDecodersH

#include <windows.h>
#include <unknwn.h>
#include "apiCore.h"
#include "apiObjects.h"
#include "apiFileManager.h"

static const GUID IID_IAIMPAudioDecoder = {0x41494D50, 0x4175, 0x6469, 0x6F, 0x44, 0x65, 0x63, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPAudioDecoderBufferingProgress = {0x41494D50, 0x4175, 0x6469, 0x6F, 0x44, 0x65, 0x63, 0x42, 0x75, 0x66, 0x66};
static const GUID IID_IAIMPExtensionAudioDecoder = {0x41494D50, 0x4578, 0x7441, 0x75, 0x64, 0x69, 0x6F, 0x44, 0x65, 0x63, 0x00};
static const GUID IID_IAIMPExtensionAudioDecoderOld = {0x41494D50, 0x4578, 0x7441, 0x75, 0x64, 0x69, 0x6F, 0x44, 0x65, 0x63, 0x4F};
static const GUID IID_IAIMPExtensionAudioDecoderPriority = {0x41494D50, 0x4578, 0x7444, 0x65, 0x63, 0x50, 0x72, 0x69, 0x6F, 0x72, 0x00};
static const GUID IID_IAIMPServiceAudioDecoders = {0x41494D50, 0x5372, 0x7641, 0x75, 0x64, 0x69, 0x6F, 0x44, 0x65, 0x63, 0x00};
static const GUID IID_IAIMPAudioDecoderListener = {0x41494D50, 0x4175, 0x6469, 0x6F, 0x44, 0x65, 0x63, 0x4C, 0x73, 0x74, 0x00};
static const GUID IID_IAIMPAudioDecoderNotifications = {0x41494D50, 0x4175, 0x6469, 0x6F, 0x44, 0x65, 0x63, 0x4E, 0x74, 0x66, 0x79};

const int AIMP_DECODER_SAMPLEFORMAT_08BIT      = 1;
const int AIMP_DECODER_SAMPLEFORMAT_16BIT      = 2;
const int AIMP_DECODER_SAMPLEFORMAT_24BIT      = 3;
const int AIMP_DECODER_SAMPLEFORMAT_32BIT      = 4;
const int AIMP_DECODER_SAMPLEFORMAT_32BITFLOAT = 5;

// Flags for IAIMPExtensionAudioDecoder / IAIMPExtensionAudioDecoderOld
const int AIMP_DECODER_FLAGS_FORCE_CREATE_INSTANCE = 0x1000;

// Flags for IAIMPAudioDecoderListener.Changed
const int AIMP_DECODER_CHANGE_INPUTFORMAT = 1;

/* IAIMPAudioDecoder */
  
class IAIMPAudioDecoder: public IUnknown
{
	public:
		virtual BOOL WINAPI GetFileInfo(IAIMPFileInfo *FileInfo) = 0;
		virtual BOOL WINAPI GetStreamInfo(int *SampleRate, int *Channels, int *SampleFormat) = 0;

		virtual BOOL WINAPI IsSeekable() = 0;
		virtual BOOL WINAPI IsRealTimeStream() = 0;

		virtual INT64 WINAPI GetAvailableData() = 0;
		virtual INT64 WINAPI GetSize() = 0;
		virtual INT64 WINAPI GetPosition() = 0;
		virtual BOOL WINAPI SetPosition(const INT64 Value) = 0;

		virtual int WINAPI Read(void *Buffer, int Count) = 0;
};

/* IAIMPAudioDecoderBufferingProgress */

class IAIMPAudioDecoderBufferingProgress: public IUnknown
{
	public:
		virtual BOOL WINAPI Get(double* Value) = 0;
};

/* IAIMPAudioDecoderListener */
class IAIMPAudioDecoderListener: public IUnknown
{
	public:
		virtual void WINAPI Changed(int Changes) = 0;
};

/* IAIMPAudioDecoderNotifications */

class IAIMPAudioDecoderNotifications: public IUnknown
{
	public:
		virtual void WINAPI ListenerAdd(IAIMPAudioDecoderListener* Listener) = 0;
		virtual void WINAPI ListenerRemove(IAIMPAudioDecoderListener* Listener) = 0;
};

/* IAIMPExtensionAudioDecoder */
  
class IAIMPExtensionAudioDecoder: public IUnknown
{
	public:
		virtual HRESULT WINAPI CreateDecoder(IAIMPStream *Stream, DWORD Flags,
			IAIMPErrorInfo *ErrorInfo, IAIMPAudioDecoder **Decoder) = 0;
};

/* IAIMPExtensionAudioDecoderOld */

class IAIMPExtensionAudioDecoderOld: public IUnknown
{
	public:
		virtual HRESULT WINAPI CreateDecoder(IAIMPString *FileName, DWORD Flags,
			IAIMPErrorInfo *ErrorInfo, IAIMPAudioDecoder **Decoder) = 0;
};

/* IAIMPExtensionAudioDecoderPriority */

class IAIMPExtensionAudioDecoderPriority: public IUnknown
{
	public:
		virtual int WINAPI GetPriority() = 0;
};

/* IAIMPServiceAudioDecoders */

class IAIMPServiceAudioDecoders: public IUnknown
{
	public:
		virtual HRESULT WINAPI CreateDecoderForStream(IAIMPStream *Stream, DWORD Flags,
			IAIMPErrorInfo *ErrorInfo, IAIMPAudioDecoder **Decoder) = 0;
		virtual HRESULT WINAPI CreateDecoderForFileURI(IAIMPString *FileURI, DWORD Flags,
			IAIMPErrorInfo *ErrorInfo, IAIMPAudioDecoder **Decoder) = 0;
};
#endif