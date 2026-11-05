// Compile-only control/effect context, not a partial native implementation.
// The name-buffer capacity is a source hypothesis; EH context remains open.
#include "SecondaryActorInstructionControl.hpp"
using namespace SecondaryHostInstructions;
extern "C" int __stdcall SecondaryHostControlProbe(SecondaryActorStateView *self,
    SecondaryInstruction *instruction)
{
    switch (instruction->opcode)
    {
#include "SecondaryActorInstructionControl.inc"
    }
    return 0;
}
