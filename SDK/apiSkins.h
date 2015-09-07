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

#ifndef apiSkinsH
#define apiSkinsH

#include <windows.h>
#include <unknwn.h>
#include "apiObjects.h"
#include "apiCore.h"

static const GUID IID_IAIMPSkinInfo = {0x41494D50, 0x536B, 0x696E, 0x49, 0x6E, 0x66, 0x6F, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPServiceSkinsManager = {0x41494D50, 0x5372, 0x7653, 0x6B, 0x69, 0x6E, 0x73, 0x4D, 0x6E, 0x67, 0x72};

// SkinInfo Properties
const int AIMP_SKININFO_PROPID_NAME         = 1;
const int AIMP_SKININFO_PROPID_AUTHOR       = 2;
const int AIMP_SKININFO_PROPID_DESCRIPTION  = 3;
const int AIMP_SKININFO_PROPID_PREVIEW      = 4;

// SkinsManager Properties
const int AIMP_SERVICE_SKINSMAN_PROPID_SKIN          = 1;
const int AIMP_SERVICE_SKINSMAN_PROPID_HUE           = 2;
const int AIMP_SERVICE_SKINSMAN_PROPID_HUE_INTENSITY = 3;

// Flags for IAIMPServiceSkinsManager.Install
const int AIMP_SERVICE_SKINSMAN_FLAGS_INSTALL_FOR_ALL_USERS = 1;

/* IAIMPSkinInfo */

class IAIMPSkinInfo: public IAIMPPropertyList
{

};

/* IAIMPServiceSkinsManager */

class IAIMPServiceSkinsManager: public IUnknown
{
	public:
		virtual HRESULT WINAPI EnumSkins(IAIMPObjectList **List) = 0;
		virtual HRESULT WINAPI GetSkinInfo(IAIMPString* FileName, IAIMPSkinInfo **Info) = 0;
		virtual HRESULT WINAPI Select(IAIMPString* FileName) = 0;
		//
		virtual HRESULT WINAPI Install(IAIMPString *FileName, DWORD Flags) = 0;
		virtual HRESULT WINAPI Uninstall(IAIMPString *FileName) = 0;
		// Tools
		virtual HRESULT WINAPI HSLToRGB(byte H, byte S, byte L, byte *R, byte *G, byte *B) = 0;
		virtual HRESULT WINAPI RGBToHSL(byte R, byte G, byte B, byte *H, byte *S, byte *L) = 0;
};

#endif // !apiSkinsH