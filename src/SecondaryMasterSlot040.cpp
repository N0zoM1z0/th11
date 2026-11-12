#include "SecondaryActorState.hpp"
#include "AnimationVm.hpp"

// Complete original 0x410530..0x41053A slot write, using only the observed
// two values and global master table. The exact original register allocator
// remains a separate non-COFF LTCG question.
void __fastcall StoreSecondaryHostSlot040(AnimationFileView *file, int index)
{
    AnimationFileView **slots = reinterpret_cast<AnimationFileView **>(
        reinterpret_cast<unsigned char *>(g_SecondaryMaster) + 0x40);
    slots[index] = file;
}
