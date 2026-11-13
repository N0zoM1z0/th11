#include "../src/SecondaryReplayState.hpp"

// The native allocator is called with its state and stage in EBX/EDI, not on
// the stack. This probe preserves that register ABI while exposing the symbol
// to the public linked-PE owner receipt.
extern "C" void __stdcall ReplayAppendWriteSegmentProbe(
    ReplayStateView *state, int stage)
{
    __asm {
        mov ebx, state
        mov edi, stage
        call AppendReplayWriteSegment
    }
}
