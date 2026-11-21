#include "AnimationExecution.hpp"

extern "C" int __stdcall AnimationExecutionProbe(AnimationVmView *vm)
{
    return ExecuteAnimationVm(vm);
}
