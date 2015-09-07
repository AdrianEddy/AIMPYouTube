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

#ifndef apiCoreH
#define apiCoreH

#include <windows.h>
#include <unknwn.h>
#include "apiObjects.h"

static const GUID IID_IAIMPCore               = {0x41494D50, 0x436F, 0x7265, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPServiceConfig      = {0x41494D50, 0x5372, 0x7643, 0x66, 0x67, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPServiceShutdown    = {0x41494D50, 0x5372, 0x7653, 0x68, 0x75, 0x74, 0x64, 0x6F, 0x77, 0x6E, 0x00};
static const GUID IID_IAIMPServiceVersionInfo = {0x41494D50, 0x5372, 0x7656, 0x65, 0x72, 0x49, 0x6E, 0x66, 0x6F, 0x00, 0x00};

static const GUID IID_IAIMPServiceAttrExtendable = {0x41494D50, 0x5372, 0x7641, 0x74, 0x74, 0x72, 0x45, 0x78, 0x74, 0x00, 0x00};
static const GUID IID_IAIMPServiceAttrObjects 	 = {0x41494D50, 0x5372, 0x7641, 0x74, 0x74, 0x72, 0x4F, 0x62, 0x6A, 0x73, 0x00};

/* IAIMPServiceVersionInfo.GetBuildState */
const int AIMP_SERVICE_VERSION_STATE_RELEASE           = 0;
const int AIMP_SERVICE_VERSION_STATE_RELEASE_CANDIDATE = 1;
const int AIMP_SERVICE_VERSION_STATE_BETA              = 2;
const int AIMP_SERVICE_VERSION_STATE_ALPHA             = 3;

/* IAIMPServiceShutdown.Shutdown */
const int AIMP_SERVICE_SHUTDOWN_FLAGS_HIBERNATE  = 0x1;
const int AIMP_SERVICE_SHUTDOWN_FLAGS_POWEROFF   = 0x2;
const int AIMP_SERVICE_SHUTDOWN_FLAGS_SLEEP      = 0x3;
const int AIMP_SERVICE_SHUTDOWN_FLAGS_REBOOT     = 0x4;
const int AIMP_SERVICE_SHUTDOWN_FLAGS_LOGOFF     = 0x5;
const int AIMP_SERVICE_SHUTDOWN_FLAGS_CLOSE_APP  = 0x10;
const int AIMP_SERVICE_SHUTDOWN_FLAGS_NO_CONFIRM = 0x20;

/* IAIMPCore.GetPath */
const int AIMP_CORE_PATH_AUDIOLIBRARY = 6;
const int AIMP_CORE_PATH_ENCODERS     = 8;
const int AIMP_CORE_PATH_HELP         = 9;
const int AIMP_CORE_PATH_ICONS        = 5;
const int AIMP_CORE_PATH_LANGS        = 2;
const int AIMP_CORE_PATH_PLAYLISTS    = 1;
const int AIMP_CORE_PATH_PLUGINS      = 4;
const int AIMP_CORE_PATH_PROFILE      = 0;
const int AIMP_CORE_PATH_SKINS        = 3;
const int AIMP_CORE_PATH_SKINS_COMMON = 11;

/* IAIMPCore */

class IAIMPCore: public IUnknown
{
	public:
		// Creating Simple Objects
		virtual HRESULT WINAPI CreateObject(REFIID IID, void **Obj) = 0;
		// System Paths
		virtual HRESULT WINAPI GetPath(int PathID, IAIMPString **Value) = 0;
		// Registration
		virtual HRESULT WINAPI RegisterExtension(REFIID ServiceIID, IUnknown *Extension) = 0;
		virtual HRESULT WINAPI RegisterService(IUnknown *Service) = 0;	
		virtual HRESULT WINAPI UnregisterExtension(IUnknown *Extension) = 0;	
};

/* IAIMPServiceAttrObjects */

class IAIMPServiceAttrObjects: public IUnknown
{
	public:
		virtual HRESULT WINAPI CreateObject(REFIID IID, void **Obj) = 0;
};

/* IAIMPServiceAttrExtendable */

class IAIMPServiceAttrExtendable: public IUnknown
{
	public:
	    virtual void WINAPI RegisterExtension(IUnknown *Extension) = 0;
		virtual void WINAPI UnregisterExtension(IUnknown *Extension) = 0;
};

/* IAIMPServiceConfig */

class IAIMPServiceConfig: public IAIMPConfig
{
	public:
		virtual HRESULT WINAPI FlushCache() = 0;
};

/* IAIMPServiceShutdown */

class IAIMPServiceShutdown: public IUnknown
{
	public:
		virtual HRESULT WINAPI Restart(IAIMPString *Params) = 0;
		virtual HRESULT WINAPI Shutdown(DWORD Flags) = 0;
};

/* IAIMPServiceVersionInfo */

class IAIMPServiceVersionInfo: public IUnknown
{
	public:
		virtual HRESULT WINAPI FormatInfo(IAIMPString **S) = 0;
		virtual int WINAPI GetBuildDate() = 0;
		virtual int WINAPI GetBuildState() = 0;
		virtual int WINAPI GetBuildNumber() = 0;
		virtual int WINAPI GetVersionID() = 0;
};

#endif // !apiCoreH