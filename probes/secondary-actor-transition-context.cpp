// Independent input; this diagnostic image is never executed.
#include "SecondaryActorRuntime.hpp"
extern "C" const char *__stdcall SecondaryActorTransitionProbe(SecondaryActorManagerView *self)
{ return SelectSecondaryActorTransition(self); }
