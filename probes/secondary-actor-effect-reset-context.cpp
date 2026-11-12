#include "../src/SecondaryActorInstructionControl.hpp"
extern "C" void __stdcall SecondaryActorEffectResetProbe(
    SecondaryActorEffectView *effect, const CameraVector *origin)
{
    ResetSecondaryStateWithEffect(effect, origin);
}
