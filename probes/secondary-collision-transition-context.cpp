// Independent real API inputs for a non-runnable code-generation context.
#include "SecondaryPlayerController.hpp"
#include "AnimationVmFactory.hpp"
#include "MeshEffects.hpp"
extern "C" AnimationVmView *__stdcall SecondaryCollisionTransitionProbe(
    SecondaryCollisionReferencesView *self, RenderBatchView *renderer, AnimationVmId id,
    AnimationFileView *file, int script, AnimationVmView *vm)
{
    BeginSecondaryCollision(self);
    InitializeSceneAnimationVm(vm, file, script);
    PublishSceneAnimationVm(vm);
    BindAnimationScript(file, script, vm);
    QueueMeshEffectSound(script);
    return FindAnimationVmById(renderer, id);
}
