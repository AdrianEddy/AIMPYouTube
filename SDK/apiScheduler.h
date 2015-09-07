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

#ifndef apiSchedulerH
#define apiSchedulerH

#include <windows.h>
#include <unknwn.h>
#include "apiObjects.h"

static const GUID IID_IAIMPSchedulerEvent = {0x41494D50, 0x5363, 0x6865, 0x64, 0x75, 0x6C, 0x65, 0x72, 0x45, 0x76, 0x74};
static const GUID IID_IAIMPServiceScheduler = {0x41494D50, 0x5372, 0x7653, 0x63, 0x68, 0x65, 0x64, 0x75, 0x6C, 0x65, 0x72};

// IAIMPServiceScheduler.GetEvent
const int AIMP_SCHEDULER_EVENT_ALARM_ID    = 0;
const int AIMP_SCHEDULER_EVENT_SHUTDOWN_ID = 1;

// PropertyID for the IAIMPSchedulerEvent
const int AIMP_SCHEDULER_EVENT_PROPID_ID                       = 1;
const int AIMP_SCHEDULER_EVENT_PROPID_ACTIVE                   = 2;
const int AIMP_SCHEDULER_EVENT_PROPID_TIME                     = 3;
const int AIMP_SCHEDULER_EVENT_PROPID_SHUTDOWN_ACTION          = 4;
const int AIMP_SCHEDULER_EVENT_PROPID_SHUTDOWN_SOURCE_EVENT    = 5;
const int AIMP_SCHEDULER_EVENT_PROPID_SHUTDOWN_TRACKCOUNT      = 6;
const int AIMP_SCHEDULER_EVENT_PROPID_ALARM_SOURCE             = 7;
const int AIMP_SCHEDULER_EVENT_PROPID_ALARM_SOURCE_TYPE        = 8;
const int AIMP_SCHEDULER_EVENT_PROPID_ALARM_VOLUME             = 9
const int AIMP_SCHEDULER_EVENT_PROPID_ALARM_VOLUME_FADING      = 10;
const int AIMP_SCHEDULER_EVENT_PROPID_ALARM_VOLUME_FADING_TIME = 11;

// Values for the AIMP_SCHEDULER_EVENT_PROPID_SOURCE_TYPE property
const int AIMP_ALARM_EVENT_SOURCE_TYPE_FILE                = 0;
const int AIMP_ALARM_EVENT_SOURCE_TYPE_PLAYLIST            = 1;

// Values for the AIMP_SCHEDULER_EVENT_PROPID_SHUTDOWN_SOURCE_EVENT property
const int AIMP_SHUTDOWN_EVENT_SOURCE_EVENT_BY_TIME         = 0;
const int AIMP_SHUTDOWN_EVENT_SOURCE_EVENT_END_OF_PLAYLIST = 1;
const int AIMP_SHUTDOWN_EVENT_SOURCE_EVENT_END_OF_QUEUE    = 2;
const int AIMP_SHUTDOWN_EVENT_SOURCE_EVENT_END_OF_TRACK    = 3;

// Values for the AIMP_SCHEDULER_EVENT_PROPID_SHUTDOWN_ACTION property
const int AIMP_SHUTDOWN_EVENT_ACTION_PAUSE_PLAYBACK             = 1;
const int AIMP_SHUTDOWN_EVENT_ACTION_SHUTDOWN_PLAYER            = 2;
const int AIMP_SHUTDOWN_EVENT_ACTION_SHUTDOWN_WINDOWS           = 4;
const int AIMP_SHUTDOWN_EVENT_ACTION_SHUTDOWN_WINDOWS_HIBERNATE = 8;
const int AIMP_SHUTDOWN_EVENT_ACTION_SHUTDOWN_WINDOWS_SLEEP     = 16;

/* IAIMPSchedulerEvent */

class IAIMPSchedulerEvent: public IAIMPPropertyList
{
	// nothing
};

/* IAIMPServiceScheduler */

class IAIMPServiceScheduler: public IUnknown
{
	public:
		virtual HRESULT WINAPI GetEvent(int ID, IAIMPSchedulerEvent **Event) = 0;
		virtual HRESULT WINAPI GetNearestEvent(IAIMPSchedulerEvent **Event) = 0;
		virtual HRESULT WINAPI GetRemainingTimeBeforeAction(double *Value) = 0;
};

#endif // !apiSchedulerH
