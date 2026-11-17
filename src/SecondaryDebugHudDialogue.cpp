#include "SecondaryDebugHudDialogue.hpp"

// Complete original TH11 0x419F80..0x419FE4: release all eight VM IDs,
// clearing each immediately after its real animation-release call.
// Eight distinct expressions retain the original observable order.
void __stdcall DestroySecondaryHudDialogue(SecondaryHudDialogueOwnedView *self)
{
    AnimationVmId *ids = reinterpret_cast<AnimationVmId *>(self->vmIds);
    AnimationVmId &id0 = ids[0];
    ReleaseSecondaryAnimation(id0);
    id0.value = 0;
    AnimationVmId &id1 = ids[1];
    ReleaseSecondaryAnimation(id1);
    id1.value = 0;
    AnimationVmId &id2 = ids[2];
    ReleaseSecondaryAnimation(id2);
    id2.value = 0;
    AnimationVmId &id3 = ids[3];
    ReleaseSecondaryAnimation(id3);
    id3.value = 0;
    AnimationVmId &id4 = ids[4];
    ReleaseSecondaryAnimation(id4);
    id4.value = 0;
    AnimationVmId &id5 = ids[5];
    ReleaseSecondaryAnimation(id5);
    id5.value = 0;
    AnimationVmId &id6 = ids[6];
    ReleaseSecondaryAnimation(id6);
    id6.value = 0;
    AnimationVmId &id7 = ids[7];
    ReleaseSecondaryAnimation(id7);
    id7.value = 0;
}