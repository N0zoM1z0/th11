#include "../src/AnimationExecution.hpp"

// Non-runnable real-caller context. ReadAnimationFloat is the native caller
// that supplies the VM-selected random state and the signed-range maximum.
extern "C" float __stdcall AnimationRandomSignedRangeProbe(
    AnimationVmView *vm, float encoded)
{
    float (__stdcall * volatile readFloat)(AnimationVmView *, float) =
        &ReadAnimationFloat;
    return readFloat(vm, encoded);
}
