#include "../src/SecondaryEmitterAnimation.hpp"

extern "C" void __stdcall FrontDrawVmProbe(AnimationFileView *file, int script, int layer)
{
    CreateDefaultFrontSecondaryDrawAnimationVmId(file, script);
    CreateFrontSecondaryDrawAnimationVmId(file, script, layer);
}
