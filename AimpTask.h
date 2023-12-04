#pragma once

#include <functional>
#include "IUnknownInterfaceImpl.h"
#include "SDK/apiThreading.h"

class AimpTask : public IUnknownInterfaceImpl<IAIMPTask> {
public:
    typedef IUnknownInterfaceImpl<IAIMPTask> Base;

    AimpTask(std::function<void()> task) : m_task(task) {};

    virtual void WINAPI Execute(IAIMPTaskOwner* Owner) { m_task(); };

    virtual ULONG WINAPI AddRef(void) { return Base::AddRef(); }
    virtual ULONG WINAPI Release(void) { return Base::Release(); }

private:
    std::function<void()> m_task;
};
