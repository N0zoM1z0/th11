#include "../src/SecondaryEmitterAnimation.hpp"

// Independent file/position/script producer for the two drawing append factories.
extern "C" void __stdcall SecondaryDrawPositionedProbe(AnimationFileView *file,
    const CameraVector *position, int script)
{
    CreatePositionedSecondaryDrawAnimationVmId(file, position, script);
    CreateOffsetSecondaryDrawAnimationVmId(file, position, script);
}
