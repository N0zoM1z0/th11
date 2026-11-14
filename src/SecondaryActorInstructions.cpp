#include "SecondaryActorInstructionRecords.hpp"
using namespace SecondaryHostInstructions;

int SecondaryActorManagerView::ExecuteInstruction()
{
    return state.ExecuteInstruction();
}

int SecondaryActorStateView::ExecuteInstruction()
{
    SecondaryActorStateView *self = this;
    SecondaryInstruction *instruction = owner->current->instruction;
    unsigned short opcode = instruction->opcode;
    AnimationVmView *angleVm;
    int angleIndex;
    SecondaryHostRecord *record;
    int secondIndex;
    short firstCount;
    switch (static_cast<short>(opcode))
    {
#include "SecondaryActorInstructionAnimation.inc"
#include "SecondaryActorInstructionMotion.inc"
#include "SecondaryActorInstructionControl.inc"
#include "SecondaryActorInstructionRecords.inc"
    }
    return 0;
}
