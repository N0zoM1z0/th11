#include "AnimationVm.hpp"

// Small stage/scene binding adapter at TH11 0x00405B10. The original
// optimized caller passes VM, script and document in private registers;
// this normal C++ signature intentionally leaves register assignment to VC9.
void __stdcall ResetAndBindStageAnimationVm(AnimationVmView *vm,
    AnimationFileView *file, int scriptId)
{
    ResetAnimationVm(vm);
    reinterpret_cast<unsigned char *>(vm)[0x41d] = 0x10;
    reinterpret_cast<unsigned char *>(vm)[0x41c] = 0x10;
    BindAnimationScript(file, scriptId, vm);
}
