#include "StageAuxiliary.hpp"
#include "StageMesh.hpp"
#include "MeshEffects.hpp"
#include "SecondaryActorRuntime.hpp"

// Both medium-sized mode dispatches are original TH11 game code. Their real
// 0x407B50 and 0x4073B0 callees remain separately owned and source-open.
void __stdcall EndSecondarySpecialState(int value);
int __stdcall UpdateStageAuxiliaryMode1Effect(StageAuxiliaryView *self);
int __stdcall UpdateStageAuxiliaryMode4Effect(StageAuxiliaryView *self);
void __cdecl QueueSecondarySpecialPulse();

// 0x407AC0..0x407B2C. Mode 1's radius grows only after frame 200; at frame
// 200 it signals its current VM before changing radius, never every frame.
int __stdcall UpdateSecondarySpecialMode1(StageAuxiliaryView *self)
{
    AnimationVmView *vm = FindAnimationVmById(
        g_RenderBatch, AnimationVmId(self->animationId040));
    if (!vm)
        self->animationId040 = 0;
    EndSecondarySpecialState(40);
    if (!vm)
        return -1;

    UpdateStageAuxiliaryMode1Effect(self);
    if (self->timer014.current >= 200)
    {
        if (self->timer014.current == 200)
        {
            QueueMeshEffectSound(0x26);
            QueueSecondarySpecialPulse();
            SignalSecondaryAnimation(&self->animationId040);
        }
        self->radius += self->radiusStep;
    }
    return 0;
}

// 0x4071A0..0x40720A. Unlike mode 1, this mode advances a positional sound
// every tenth frame in (1,50), and snapshots the VM's Y scale after drawing.
int __stdcall UpdateSecondarySpecialMode4(StageAuxiliaryView *self)
{
    AnimationVmView *vm = FindAnimationVmById(
        g_RenderBatch, AnimationVmId(self->animationId040));
    if (!vm)
        self->animationId040 = 0;
    EndSecondarySpecialState(40);
    if (!vm)
        return -1;

    const int frame = self->timer014.current;
    if (frame % 10 == 0 && frame < 50 && frame > 1)
        QueueSecondaryActorSound(0x26, self->origin.x);
    UpdateStageAuxiliaryMode4Effect(self);
    self->radius = vm->scaleY;
    return 0;
}
