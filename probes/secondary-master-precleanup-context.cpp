#include "../src/SecondaryActorState.hpp"
extern void __stdcall PrepareSecondaryMasterDestruction(SecondaryMasterView *self);
extern "C" void __stdcall SecondaryMasterPrecleanupProbe(SecondaryMasterView *self)
{
    PrepareSecondaryMasterDestruction(self);
}
