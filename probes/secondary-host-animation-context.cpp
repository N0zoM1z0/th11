// Compile-only context for the creation/animation source fragment, not a
// partial native host or an executable behavior harness.
#include "SecondaryActorInstructionAnimation.hpp"
using namespace SecondaryHostInstructions;
extern "C" void __stdcall SecondaryHostAnimationProbe(SecondaryActorStateView *self,
    SecondaryInstruction *instruction)
{
    unsigned short opcode = instruction->opcode;
    AnimationVmView *angleVm;
    int angleIndex;
    switch (opcode)
    {
#include "SecondaryActorInstructionAnimation.inc"
    }
}
