// Compile-only control/effect context, not a partial native implementation.
// The enclosing native name-buffer capacity and EH lifetimes remain open.
#include "SecondaryActorInstructionControl.hpp"
using namespace SecondaryHostInstructions;
extern "C" int __stdcall SecondaryHostControlProbe(SecondaryActorStateView *self,
    SecondaryInstruction *instruction, unsigned char *decodedName)
{
    switch (instruction->opcode)
    {
#include "SecondaryActorInstructionControl.inc"
    }
    return 0;
}
