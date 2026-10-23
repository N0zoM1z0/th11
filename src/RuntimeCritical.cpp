#include "RuntimeCritical.hpp"

void __stdcall LeaveRuntimeCriticalSection(MainRuntimeView *self, int index)
{
    if (self->flags & 0x8000)
    {
        LeaveCriticalSection(&self->criticalSections[index]);
        --self->criticalDepth[index];
    }
}
