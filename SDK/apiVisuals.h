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

#ifndef apiVisualsH
#define apiVisualsH

#include <windows.h>
#include <unknwn.h>
#include "apiObjects.h"
#include "apiCore.h"

static const GUID IID_IAIMPExtensionCustomVisualization = {0x41494D50, 0x4578, 0x7443, 0x73, 0x74, 0x6D, 0x56, 0x69, 0x73, 0x00, 0x00};
static const GUID IID_IAIMPExtensionEmbeddedVisualization = {0x41494D50, 0x4578, 0x7445, 0x6D, 0x62, 0x64, 0x56, 0x69, 0x73, 0x00, 0x00};
static const GUID IID_IAIMPServiceVisualizations = {0x41494D50, 0x5372, 0x7656, 0x69, 0x73, 0x75, 0x61, 0x6C, 0x00, 0x00, 0x00};

// Button ID for IAIMPExtensionEmbeddedVisualization.Click
const int AIMP_VISUAL_CLICK_BUTTON_LEFT   = 0;
const int AIMP_VISUAL_CLICK_BUTTON_MIDDLE = 1;

// flags for IAIMPExtensionEmbeddedVisualization.GetFlags and IAIMPExtensionCustomVisualization.GetFlags
const int AIMP_VISUAL_FLAGS_RQD_DATA_WAVE       = 1;
const int AIMP_VISUAL_FLAGS_RQD_DATA_SPECTRUM   = 2;
const int AIMP_VISUAL_FLAGS_NOT_SUSPEND	     = 4;

const int AIMP_VISUAL_SPECTRUM_MAX = 256;
const int AIMP_VISUAL_WAVEFORM_MAX = 512;

typedef float TAIMPVisualDataSpectrum[AIMP_VISUAL_SPECTRUM_MAX];
typedef float TAIMPVisualDataWaveform[AIMP_VISUAL_WAVEFORM_MAX];

#pragma pack(push, 1)
struct TAIMPVisualData
{
	float Peaks[2];
	TAIMPVisualDataSpectrum Spectrum[3];
	TAIMPVisualDataWaveform WaveForm[2];
	int Reserved;
};
#pragma pack(pop)
typedef TAIMPVisualData* PAIMPVisualData;

/* IAIMPVisualExtension */

class IAIMPExtensionCustomVisualization: public IUnknown
{
		// Common Information
		virtual int WINAPI GetFlags() = 0;
		// Basic functionality
		virtual void WINAPI Draw(PAIMPVisualData Data) = 0;
};

/* IAIMPExtensionEmbeddedVisualization */

class IAIMPExtensionEmbeddedVisualization: public IUnknown
{
	public:
		// Common Information
		virtual int WINAPI GetFlags() = 0;
		virtual HRESULT WINAPI GetMaxDisplaySize(int *Width, int *Height) = 0;
		virtual HRESULT WINAPI GetName(IAIMPString **S) = 0;
		// Initialization / Finalization
		virtual void WINAPI Initialize(int Width, int Height) = 0;
		virtual void WINAPI Finalize() = 0;
		// Basic functionality
		virtual void WINAPI Click(int X, int Y, int Button) = 0;
		virtual void WINAPI Draw(HDC DC, PAIMPVisualData Data) = 0;
		virtual void WINAPI Resize(int NewWidth, int NewHeight) = 0;
};

/* IAIMPServiceVisual */

class IAIMPServiceVisualizations: public IUnknown
{
};

#endif // !apiVisualsH