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

#ifndef apiDecodersH
#define apiDecodersH

#include <windows.h>
#include <unknwn.h>
#include "apiCore.h"
#include "apiObjects.h"
#include "apiFileManager.h"

static const GUID IID_IAIMPAudioDecoder = {0x41494D50, 0x4175, 0x6469, 0x6F, 0x44, 0x65, 0x63, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPExtensionAudioDecoder = {0x41494D50, 0x4578, 0x7441, 0x75, 0x64, 0x69, 0x6F, 0x44, 0x65, 0x63, 0x00};
static const GUID IID_IAIMPExtensionAudioDecoderOld = {0x41494D50, 0x4578, 0x7441, 0x75, 0x64, 0x69, 0x6F, 0x44, 0x65, 0x63, 0x4F};
static const GUID IID_IAIMPServiceAudioDecoders = {0x41494D50, 0x5372, 0x7641, 0x75, 0x64, 0x69, 0x6F, 0x44, 0x65, 0x63, 0x00};

const int AIMP_DECODER_SAMPLEFORMAT_08BIT      = 1;
const int AIMP_DECODER_SAMPLEFORMAT_16BIT      = 2;
const int AIMP_DECODER_SAMPLEFORMAT_24BIT      = 3;
const int AIMP_DECODER_SAMPLEFORMAT_32BIT      = 4;
const int AIMP_DECODER_SAMPLEFORMAT_32BITFLOAT = 5;

// Flags for IAIMPExtensionAudioDecoder / IAIMPExtensionAudioDecoderOld
const int AIMP_DECODER_FLAGS_FORCE_CREATE_INSTANCE = 0x1000;

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
  
/* IAIMPAudioDecoderExtension */
  
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