#include "FrameTickWorker.hpp"
#include "FrameClock.hpp"
#include <windows.h>

// Source-present, nonexact reconstruction of the original 181-byte
// 0x443140..0x4431F4 timer-worker loop. Its x87 scheduling/ABI is pending.
void __cdecl RunFrameTickWorker()
{
    g_FrameTickWorker.flags |= 2;
    while (!(g_FrameTickWorker.flags & 1))
    {
        double now;
        double target;
        do
        {
            now = GetFrameClockSeconds();
            g_FrameTickWorker.currentSeconds = now;
            if (g_FrameTickWorker.priorSeconds > now)
            {
                target = now;
                g_FrameTickWorker.nextTickSeconds = now;
            }
            else
            {
                target = g_FrameTickWorker.nextTickSeconds;
            }
            g_FrameTickWorker.priorSeconds = now;
        } while (now <= target);
        if (target < now)
        {
            do { target += 1.0 / 60.0; } while (target < now);
            g_FrameTickWorker.nextTickSeconds = target;
        }
        ++g_FrameTickWorker.completedTicks;
        Sleep(13);
    }
}
