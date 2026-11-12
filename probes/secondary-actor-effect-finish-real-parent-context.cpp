#include "../src/SecondaryActorState.hpp"
#include "../src/SecondaryActor.hpp"
extern "C" int __stdcall SecondaryActorEffectFinishRealParentProbe(SecondaryActorStateView *state)
{
    return UpdateSecondaryActorState(state);
}
