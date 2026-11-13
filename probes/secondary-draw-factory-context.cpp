#include "../src/SecondaryEmitterAnimation.hpp"

extern "C" void __stdcall SecondaryDrawFactoryProbe(
    AnimationFileView *file, int script, int layer)
{
    CreateSecondaryDrawAnimationVmId(file, script, layer);
}
