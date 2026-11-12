#include "ThreadInfLifecycle.hpp"
#include <new>

// Compile-only dynamic producer: the three independent lifetime operations
// are deliberately exclusive, not performed sequentially on one object.
extern "C" void __stdcall ThreadInfLifecycleProbe(ThreadInfView *self, int action)
{
    if (action == 0)
        new (self) ThreadInfView;
    else if (action == 1)
        self->~ThreadInfView();
    else
        delete self;
}
