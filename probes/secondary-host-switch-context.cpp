// All handled host opcodes in one compile-only switch. The real member entry is
// in SecondaryActorInstructions.cpp; this probe supplies a captured instruction.
#include "SecondaryActorInstructionRecords.hpp"
using namespace SecondaryHostInstructions;
extern "C" int __stdcall SecondaryHostSwitchProbe(SecondaryActorStateView *self,
    SecondaryInstruction *instruction)
{
    short opcode = instruction->opcode;
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
