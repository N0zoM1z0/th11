#include "../src/SecondaryEmitterAnimation.hpp"

extern "C" void __stdcall SecondaryDrawPublisherProbe(
    AnimationVmView *vm, AnimationVmId *result)
{
    __asm {
        push ebx
        mov ebx, vm
        mov eax, result
        call PublishSecondaryDrawAnimationVmNative
        pop ebx
    }
}
