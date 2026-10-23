#include "FrameClock.hpp"
#include "MainRuntime.hpp"

#include <mmsystem.h>

double __cdecl GetFrameClockSeconds()
{
    double seconds;
    LARGE_INTEGER counter;

    if (g_MainRuntime.flags & 0x8000)
    {
        EnterCriticalSection(&g_MainRuntime.criticalSections[5]);
        ++g_MainRuntime.criticalDepth[5];
    }

    if (g_FrameClockFrequency != 0)
    {
        QueryPerformanceCounter(&counter);
        seconds = double(counter.QuadPart - g_FrameClockCounterBase) /
                  double(g_FrameClockFrequency);
        if (g_FrameClockOrigin > seconds)
            g_FrameClockOrigin = seconds;

        if (g_MainRuntime.flags & 0x8000)
        {
            LeaveCriticalSection(&g_MainRuntime.criticalSections[5]);
            --g_MainRuntime.criticalDepth[5];
        }
        return seconds - g_FrameClockOrigin;
    }

    seconds = timeGetTime();
    if (seconds < g_FrameClockOrigin)
        g_FrameClockOrigin = seconds;
    seconds = (seconds - g_FrameClockOrigin * 1000.0) / 1000.0;
    if (g_MainRuntime.flags & 0x8000)
    {
        LeaveCriticalSection(&g_MainRuntime.criticalSections[5]);
        --g_MainRuntime.criticalDepth[5];
    }
    return seconds;
}
