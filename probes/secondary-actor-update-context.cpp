// Independent input; this diagnostic image is never executed.
#include "SecondaryActorRuntime.hpp"
extern "C" int __stdcall SecondaryActorUpdateProbe(SecondaryActorStateView *self)
{ return UpdateSecondaryActorState(self); }
