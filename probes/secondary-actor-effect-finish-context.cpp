#include "../src/SecondaryActorState.hpp"
extern "C" int __stdcall SecondaryActorEffectFinishProbe(SecondaryActorManagerView *actor)
{
    return FinishSecondaryActorEffect(actor);
}
