// Keep the real manager-to-state member call visible in the diagnostic link.
// This entry is not a native game entrypoint or a runtime harness.
#include "SecondaryActorState.hpp"
extern "C" int __stdcall SecondaryHostOwnerProbe(SecondaryActorManagerView *self)
{
    return self->SecondaryActorManagerView::ExecuteInstruction();
}
