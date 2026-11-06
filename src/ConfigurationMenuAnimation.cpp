#include "ConfigurationAudioMenu.hpp"
#include "SecondaryPlayerMovement.hpp"
#include "VmBindings.hpp"

void __stdcall SignalAndExecuteConfigurationAnimation(AnimationVmId id, short event)
{
    AnimationVmView *vm = FindAnimationVmById(g_RenderBatch, id);
    if (!vm) return;
    reinterpret_cast<SecondaryActorAnimationView *>(vm)->event = event;
    ExecuteAnimationVm(vm);
    BoundVmView *bound = reinterpret_cast<BoundVmView *>(vm);
    if (!bound->marker018)
        for (BoundVmNodeView *node = bound->related; node; node = node->next)
        {
            reinterpret_cast<SecondaryActorAnimationView *>(node->vm)->event = event;
            ExecuteAnimationVm(reinterpret_cast<AnimationVmView *>(node->vm));
        }
}
