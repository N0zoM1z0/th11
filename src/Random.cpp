#include "Random.hpp"
#include "MainRuntime.hpp"

unsigned short __stdcall RandomNext16(RandomStateView *self)
{
    if (g_MainRuntime.flags & 0x8000)
    {
        EnterCriticalSection(&g_MainRuntime.criticalSections[10]);
        ++g_MainRuntime.criticalDepth[10];
    }

    ++self->calls;
    unsigned short value = (self->seed ^ 0x9630) - 0x6553;
    self->seed = (value >> 14) + (value << 2);

    if (g_MainRuntime.flags & 0x8000)
    {
        LeaveCriticalSection(&g_MainRuntime.criticalSections[10]);
        --g_MainRuntime.criticalDepth[10];
    }
    return self->seed;
}

unsigned int __stdcall RandomNext32(RandomStateView *self)
{
    if (g_MainRuntime.flags & 0x8000)
    {
        EnterCriticalSection(&g_MainRuntime.criticalSections[10]);
        ++g_MainRuntime.criticalDepth[10];
    }

    unsigned short high = (self->seed ^ 0x9630) - 0x6553;
    self->calls += 2;
    self->seed = (high >> 14) + (high << 2);
    unsigned short low = (self->seed ^ 0x9630) - 0x6553;
    self->seed = (low >> 14) + (low << 2);

    if (g_MainRuntime.flags & 0x8000)
    {
        LeaveCriticalSection(&g_MainRuntime.criticalSections[10]);
        --g_MainRuntime.criticalDepth[10];
    }
    return (unsigned int(high) << 16) | low;
}
