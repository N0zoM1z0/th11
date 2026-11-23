#include "../src/AnimationExecution.hpp"
#include "../src/MainRuntime.hpp"

// Non-runnable real-caller context for ReadAnimationFloat operand 10010.
// The context helper is a dead-strippable LTCG twin of the interpreter reader.
float __stdcall ReadAnimationFloatContext(AnimationVmView *vm, float encoded);

extern "C" float __stdcall AnimationRandomSignedRangeProbe(
    AnimationVmView *vm, float encoded)
{
    float (__stdcall * volatile readFloat)(AnimationVmView *, float) =
        &ReadAnimationFloatContext;
    return readFloat(vm, encoded);
}
