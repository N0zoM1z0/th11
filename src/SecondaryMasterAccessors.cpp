#include "SecondaryActorState.hpp"
#include "AnimationVm.hpp"

// Complete 0x410540..0x410548: return the first secondary actor list node.
SecondaryActorNode *__cdecl ReadSecondaryHost068()
{
    return g_SecondaryMaster->first;
}

// Complete 0x410550..0x410559: update the indexed 0x1C actor slot.
// The original two inputs naturally use ECX/EDX under fastcall.
void __fastcall StoreSecondaryHostSlot01C(SecondaryActorManagerView *actor, int index)
{
    g_SecondaryMaster->Slot(index) = actor;
}

// Complete 0x410560..0x410574: set only the low bit of master flag +0x3C.
void __stdcall SetSecondaryHostFlag03C(unsigned int bit)
{
    unsigned int *flags = reinterpret_cast<unsigned int *>(reinterpret_cast<unsigned char *>(g_SecondaryMaster) + 0x3c);
    *flags = (*flags & ~1u) | (bit & 1u);
}
