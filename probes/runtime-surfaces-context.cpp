// Non-runnable context preserving independent runtime, animation and VM inputs.
#include "../src/RuntimeSurfaces.hpp"

extern "C" void __stdcall RuntimeSurfacesProbe(MainRuntimeView *first, MainRuntimeView *second,
    AnimationFileView *file, AnimationVmView *vm, int scriptId)
{
    ResetAnimationVm(vm);
    BindAnimationScript(file, scriptId, vm);
    EnsureRuntimeSurfaces(first);
    EnsureRuntimeSurfaces(second);
    ReleaseRuntimeSurfaces();
}
