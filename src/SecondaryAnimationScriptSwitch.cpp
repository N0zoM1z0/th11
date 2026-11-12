#include "SecondaryActorRuntime.hpp"
#include "AnimationVmFactory.hpp"

// Original complete 0x456770..0x456793 leaf. The source-level signature
// agrees with the existing secondary actor API. Under original optimized
// ownership the id-pointer is supplied in EAX and the script on the stack.
void __stdcall ChangeSecondaryAnimationScript(unsigned int *id, int script)
{
    AnimationVmView *vm = FindAnimationVmById(g_RenderBatch, AnimationVmId(*id));
    if (vm)
        BindAnimationFileScript(vm->file, script, vm);
}
