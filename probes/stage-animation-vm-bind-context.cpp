#include "../src/AnimationVm.hpp"

void __stdcall ResetAndBindStageAnimationVm(AnimationVmView *vm,
    AnimationFileView *file, int scriptId);

extern "C" __declspec(dllexport) void __stdcall StageAnimationVmBindProbe(
    AnimationVmView *vm, AnimationFileView *file, int scriptId)
{
    ResetAndBindStageAnimationVm(vm, file, scriptId);
}
