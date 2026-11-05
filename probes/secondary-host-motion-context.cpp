// Compile-only context for the motion source fragment. This is not a partial
// implementation of the native host and grants no complete-owner match credit.
#include "SecondaryActorInstructions.hpp"
using namespace SecondaryHostInstructions;
extern "C" void __stdcall SecondaryHostMotionProbe(SecondaryActorStateView *self,
    SecondaryInstruction *instruction)
{
    unsigned short opcode = instruction->opcode;
    switch (opcode)
    {
#include "SecondaryActorInstructionMotion.inc"
    }
}
