#include "SecondaryActorInstructionRecords.hpp"

extern "C" float __stdcall SecondaryAngleMathProbe(float y, float x)
{
    return SecondaryAngleMath(y, x);
}
