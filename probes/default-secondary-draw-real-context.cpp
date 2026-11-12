#include "../src/SecondaryEmitterAnimation.hpp"
#include "../src/PauseContinueMenu.hpp"
// An independent live script producer prevents /GL constant-only specialization.
extern AnimationVmId __stdcall CreateDefaultSecondaryDrawAnimationVmId(AnimationFileView *, int);
extern "C" void __stdcall DefaultSecondaryDrawProbe(AnimationFileView *file, int script)
{
    CreateDefaultSecondaryDrawAnimationVmId(file, script);
}
