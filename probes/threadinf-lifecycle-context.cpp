#include "ThreadInfLifecycle.hpp"
extern "C" void __stdcall ThreadInfLifecycleProbe(ThreadInfView *self)
{
    DestroyThreadInfForUnwind(self);
}
