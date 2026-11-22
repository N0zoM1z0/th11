#include "AnimationExecution.hpp"

extern "C" void __stdcall AnimationExecutionPolarPointProbe(
    AnimationExecutionPolarPoint *point, float angle, float radius)
{
    point->Set(angle, radius);
}
