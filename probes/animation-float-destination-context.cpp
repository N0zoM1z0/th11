#include "AnimationExecution.hpp"

extern "C" __declspec(dllexport) float *__stdcall AnimationFloatDestinationProbe(
    AnimationVmView *vm, float *encoded)
{
    return AnimationFloatDestination(vm, encoded);
}
