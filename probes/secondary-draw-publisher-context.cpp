#include "../src/SecondaryEmitterAnimation.hpp"
#include "../src/PauseContinueMenu.hpp"

// Non-runnable link context: the two draw-list creators share the publisher.
extern "C" int __stdcall SecondaryDrawPublisherProbe(
    AnimationFileView *file, int script)
{
    AnimationVmId first = CreateSecondaryDrawAnimationVmId(file, script, 3);
    AnimationVmId second = CreateDefaultSecondaryDrawAnimationVmId(file, script);
    return first.value + second.value;
}
