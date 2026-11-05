// Compile-only context for the remaining host cases; not a native host stub.
#include "SecondaryActorInstructionRecords.hpp"
using namespace SecondaryHostInstructions;
extern "C" int __stdcall SecondaryHostRecordsProbe(SecondaryActorStateView *self,
    SecondaryInstruction *instruction)
{
    unsigned short opcode = instruction->opcode;
    SecondaryHostRecord *record;
    int recordIndex, secondIndex;
    float firstReal;
    short firstCount;
    switch (opcode)
    {
#include "SecondaryActorInstructionRecords.inc"
    }
    return 0;
}
