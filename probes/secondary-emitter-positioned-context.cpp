#include "../src/SecondaryActorInstructionAnimation.hpp"

// Dynamically supplied file, position, and script, not a constant-only caller.
extern "C" void __stdcall PositionedSecondaryEmitterProbe(AnimationFileView *file,
    const CameraVector *position, int script)
{
    CreatePositionedSecondaryEmitterAnimationVmId(file, position, script);
    CreateOffsetSecondaryEmitterAnimationVmId(file, position, script);
}
