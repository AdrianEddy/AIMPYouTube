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

#ifndef apiInternetH
#define apiInternetH

#include <windows.h>
#include <unknwn.h>
#include "apiObjects.h"
#include "apiCore.h"

static const GUID IID_IAIMPServiceConnectionSettings = {0x4941494D, 0x5053, 0x7276, 0x43, 0x6F, 0x6E, 0x6E, 0x43, 0x66, 0x67, 0x00};
static const GUID IID_IAIMPServiceHTTPClient = {0x41494D50, 0x5372, 0x7648, 0x74, 0x74, 0x70, 0x43, 0x6C, 0x74, 0x00, 0x00};
static const GUID IID_IAIMPHTTPClientEvents = {0x41494D50, 0x4874, 0x7470, 0x43, 0x6C, 0x74, 0x45, 0x76, 0x74, 0x73, 0x00};

// PropIDs for IAIMPServiceConnectionSettings
const int AIMP_SERVICE_CONSET_PROPID_CONNECTION_TYPE = 1;
const int AIMP_SERVICE_CONSET_PROPID_PROXY_SERVER    = 2;
const int AIMP_SERVICE_CONSET_PROPID_PROXY_PORT      = 3;
const int AIMP_SERVICE_CONSET_PROPID_PROXY_USERNAME  = 4;
const int AIMP_SERVICE_CONSET_PROPID_PROXY_USERPASS  = 5;
const int AIMP_SERVICE_CONSET_PROPID_TIMEOUT         = 6;
const int AIMP_SERVICE_CONSET_PROPID_USERAGENT       = 7;

// Connection Types
const int AIMP_SERVICE_CONSET_CONNECTIONTYPE_DIRECT         = 0;
const int AIMP_SERVICE_CONSET_CONNECTIONTYPE_PROXY          = 1;
const int AIMP_SERVICE_CONSET_CONNECTIONTYPE_SYSTEMDEFAULTS = 2;

// Flags for HTTPClient
const int AIMP_SERVICE_HTTPCLIENT_FLAGS_WAITFOR         = 1;
const int AIMP_SERVICE_HTTPCLIENT_FLAGS_UTF8            = 2;
const int AIMP_SERVICE_HTTPCLIENT_FLAGS_PRIORITY_NORMAL = 0;
const int AIMP_SERVICE_HTTPCLIENT_FLAGS_PRIORITY_LOW    = 4;
const int AIMP_SERVICE_HTTPCLIENT_FLAGS_PRIORITY_HIGH   = 8;

/* IAIMPHTTPClientEvents */

class IAIMPHTTPClientEvents: public IUnknown
{
	public:
		virtual void WINAPI OnAccept(IAIMPString *ContentType, const INT64 ContentSize, BOOL *Allow) = 0;
		virtual void WINAPI OnComplete(IAIMPErrorInfo *ErrorInfo, BOOL Canceled) = 0;
		virtual void WINAPI OnProgress(const INT64 Downloaded, const INT64 Total) = 0;
};

/* IAIMPServiceConnectionSettings */

class IAIMPServiceConnectionSettings: public IAIMPPropertyList
{
};

/* IAIMPServiceHTTPClient */

class IAIMPServiceHTTPClient: public IUnknown
{
	public:
		virtual HRESULT WINAPI Get(IAIMPString *URL, DWORD Flags, IAIMPStream *AnswerData,
			IAIMPHTTPClientEvents *EventHandler, IAIMPConfig *Params, void **TaskID) = 0;
		virtual HRESULT WINAPI Post(IAIMPString *URL, DWORD Flags, IAIMPStream *AnswerData, IAIMPStream *PostData,
			IAIMPHTTPClientEvents *EventHandler, IAIMPConfig *Params, void **TaskID) = 0;
		virtual HRESULT WINAPI Cancel(void *TaskID, DWORD Flags) = 0;
};

#endif // !apiInternetH
