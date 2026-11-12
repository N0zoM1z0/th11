#include "../src/SecondaryActorState.hpp"
extern "C" int __stdcall SecondaryActorEffectRealParentProbe(SecondaryActorStateView *state)
{
    return state->ExecuteInstruction();
}
