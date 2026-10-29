#include "SecondaryPattern.hpp"
#include "SecondaryEmitterAnimation.hpp"

extern "C" int __stdcall SecondaryEmitterResetProbe(SecondaryDamageReferencesView *self,
    AnimationFileView *file, int script, int layer, AnimationVmView *vm)
{
    ResetSecondaryPlayerEmitters(self);
    ResetSecondaryPatternSlot();
    AnimationVmId first = PublishSecondaryEmitterAnimationVm(vm);
    AnimationVmId second = CreateSecondaryEmitterAnimationVmId(file, script, layer);
    AnimationVmId third = PublishSecondaryDrawAnimationVm(vm);
    AnimationVmId fourth = CreateSecondaryDrawAnimationVmId(file, script, layer);
    return first.value + second.value + third.value + fourth.value;
}
