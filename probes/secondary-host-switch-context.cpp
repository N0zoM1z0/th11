// All handled host opcodes in one compile-only switch. The supplied workspace
// deliberately leaves the native name-array extent and enclosing ABI open.
#include "SecondaryActorInstructionRecords.hpp"
using namespace SecondaryHostInstructions;
extern "C" int __stdcall SecondaryHostSwitchProbe(SecondaryActorStateView *self,
    SecondaryInstruction *instruction, unsigned char *decodedName)
{
    short opcode = instruction->opcode;
    SecondaryHostSpawnArguments arguments;
    const SecondaryValue *values;
    AnimationVmView *angleVm;
    int angleIndex;
    SecondaryHostRecord *record;
    int recordIndex, secondIndex;
    float firstReal;
    short firstCount;
    switch (opcode)
    {
#include "SecondaryActorInstructionAnimation.inc"
#include "SecondaryActorInstructionMotion.inc"
#include "SecondaryActorInstructionControl.inc"
#include "SecondaryActorInstructionRecords.inc"
    }
    return 0;
}
