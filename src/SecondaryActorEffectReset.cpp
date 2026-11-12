#include "SecondaryActorInstructionControl.hpp"
#include "SecondaryPlayerController.hpp"

void __stdcall EmitSecondaryActorEffects(SecondaryActorEffectView *effect,
    const CameraVector *origin);

// Full native 0x00410190..0x004101C9: the 58-byte effect entry is distinct
// from its 457-byte count emission callee. Always clear the original ID.
void __stdcall ResetSecondaryStateWithEffect(SecondaryActorEffectView *effect,
    const CameraVector *origin)
{
    if (effect->id > 0)
        SpawnSecondaryPlayerDrop(g_SecondaryControllerDrops, origin, effect->id,
            0xffffffffu, -1.5707963705062866f, 2.200000047683716f);
    EmitSecondaryActorEffects(effect, origin);
    effect->id = 0;
}
