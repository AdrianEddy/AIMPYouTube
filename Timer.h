#pragma once

#include <windows.h>
#include <functional>
#include <map>

class Timer {
    typedef std::function<void()> Callback;
public:
    static void SingleShot(unsigned int milliseconds, Callback func) {
        UINT_PTR taskId = SetTimer(NULL, NULL, milliseconds, TimerProc);
        m_timers[taskId] = { func, true };
    }

    static UINT_PTR Schedule(unsigned int milliseconds, Callback func) {
        UINT_PTR taskId = SetTimer(NULL, NULL, milliseconds, TimerProc);
        m_timers[taskId] = { func, false };

        return taskId;
    }

    static void Cancel(UINT_PTR taskId) {
        KillTimer(NULL, taskId);
        m_timers.erase(taskId);
    }

    static void StopAll() {
        for (auto &x : m_timers) {
            KillTimer(NULL, x.first);
        }
        m_timers.clear();
    }

private:
    static void CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) {
        if (uMsg == WM_TIMER) {
            if (m_timers.find(idEvent) != m_timers.end()) {
                m_timers[idEvent].Callback();
                if (m_timers[idEvent].IsSingleShot) {
                    KillTimer(hwnd, idEvent);
                    m_timers.erase(idEvent);
                }
            }
        }
    }

    struct TimerInfo {
        Callback Callback;
        bool IsSingleShot;
    };
    static std::map<UINT_PTR, TimerInfo> m_timers;
};
