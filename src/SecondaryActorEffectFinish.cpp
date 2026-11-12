#include "SecondaryActorState.hpp"
#include "SecondaryActorRuntime.hpp"
#include "SecondaryPlayerController.hpp"
#include "MeshEffects.hpp"

void __stdcall EmitSecondaryActorEffects(SecondaryActorEffectView *effect,
    const CameraVector *origin);

// Full native 0x4127E0..0x41287B, 156-byte actor effect finish owner.
// The three adjacent integer slots are physically at manager+2580/2584/2588.
// Original source-level class and indexing contracts are still unresolved.
int __stdcall FinishSecondaryActorEffect(SecondaryActorManagerView *self)
{
    SecondaryActorStateView &state = self->state;
    int *settings = reinterpret_cast<int *>(state.unknown1544);
    CameraVector *position = &state.world.position;

    if (settings[0] >= 0)
        QueueSecondaryActorSound(settings[0], position->x);

    if (settings[1] >= 0)
    {
        AnimationFileView *file = *reinterpret_cast<AnimationFileView **>(
            reinterpret_cast<unsigned char *>(g_SecondaryMaster) + 0x40 +
            static_cast<unsigned int>(settings[2]) * 4);
        CreatePositionedAnimationVmId(file, position, settings[1], 3);
    }

    SecondaryActorEffectView *effect = &state.effect;
    if (effect->id > 0)
        SpawnSecondaryPlayerDrop(g_SecondaryControllerDrops, position, effect->id,
            0xffffffffu, -1.5707963705062866f, 2.200000047683716f);
    EmitSecondaryActorEffects(effect, position);
    effect->id = 0;
    return 1;
}
