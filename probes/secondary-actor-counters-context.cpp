// Independent input; this diagnostic image is never executed.
#include "SecondaryActorRuntime.hpp"
extern "C" void __stdcall SecondaryActorCountersProbe(int value)
{
    AddSecondaryActorScore(value);
    IncrementSecondaryCollisionCount();
    AddSecondaryCollisionValue(value);
}
