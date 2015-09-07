/************************************************/
/*                                              */
/*          AIMP Programming Interface          */
/*               v3.60 build 1401               */
/*                                              */
/*                Artem Izmaylov                */
/*                (C) 2006-2015                 */
/*                 www.aimp.ru                  */
/*              ICQ: 345-908-513                */
/*            Mail: support@aimp.ru             */
/*                                              */
/************************************************/

#ifndef apiThreadingH
#define apiThreadingH

#include <windows.h>
#include <unknwn.h>

static const GUID IID_IAIMPTask 		       = {0x41494D50, 0x5461, 0x736B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPTaskOwner           = {0x41494D50, 0x5461, 0x736B, 0x4F, 0x77, 0x6E, 0x65, 0x72, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPTaskPriority        = {0x41494D50, 0x5461, 0x736B, 0x50, 0x72, 0x69, 0x6F, 0x72, 0x69, 0x74, 0x79};
static const GUID IID_IAIMPServiceSynchronizer = {0x41494D50, 0x5372, 0x7653, 0x79, 0x6E, 0x63, 0x72, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPServiceThreadPool   = {0x41494D50, 0x5372, 0x7654, 0x68, 0x72, 0x64, 0x50, 0x6F, 0x6F, 0x6C, 0x00};

// Flags for IAIMPServiceThreadPool.Cancel
const int AIMP_SERVICE_THREADPOOL_FLAGS_WAITFOR = 0x1;

// IAIMPTaskPriority.GetPriority
const int AIMP_TASK_PRIORITY_NORMAL = 0;
const int AIMP_TASK_PRIORITY_LOW    = 1;
const int AIMP_TASK_PRIORITY_HIGH   = 2;

/* IAIMPTaskOwner */

class IAIMPTaskOwner: public IUnknown
{
	public:
		virtual BOOL WINAPI IsCanceled() = 0;
};

/* IAIMPTask */

class IAIMPTask: public IUnknown
{
	public:
		virtual void WINAPI Execute(IAIMPTaskOwner* Owner) = 0;
};

/* IAIMPTaskPriority */

class IAIMPTaskPriority: public IUnknown
{
	public:
		virtual int WINAPI GetPriority() = 0;
};

/* IAIMPServiceSynchronizer */

class IAIMPServiceSynchronizer: public IUnknown
{
	public:
		virtual HRESULT WINAPI ExecuteInMainThread(IAIMPTask* Task, BOOL ExecuteNow) = 0;
};

/* IAIMPServiceThreadPool */

class IAIMPServiceThreadPool: public IUnknown
{
	public:
		virtual HRESULT WINAPI Cancel(DWORD_PTR TaskHandle, DWORD Flags) = 0;
		virtual HRESULT WINAPI Execute(IAIMPTask* Task, DWORD_PTR *TaskHandle) = 0;
		virtual HRESULT WINAPI WaitFor(DWORD_PTR TaskHandle) = 0;
};

#endif // !apiThreadingH
