#include "../src/SecondaryActorState.hpp"
extern void __stdcall EmitSecondaryActorEffects(SecondaryActorEffectView *, const CameraVector *);
extern "C" void __stdcall SecondaryActorEffectsProbe(SecondaryActorEffectView *effect,
    const CameraVector *position)
{
    EmitSecondaryActorEffects(effect, position);
}
