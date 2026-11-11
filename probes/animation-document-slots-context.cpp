#include "StageResource.hpp"

void __stdcall FreeAnimationDocumentSlot(RenderBatchView *renderer, unsigned int index);
int __cdecl UpdateAnimationDocumentSlots();

// Both slot index and renderer remain dynamic; this is a compile-only caller,
// not an input generator for original TH11 executable data.
extern "C" int __stdcall AnimationDocumentSlotsProbe(
    RenderBatchView *renderer, unsigned int index)
{
    FreeAnimationDocumentSlot(renderer, index);
    return UpdateAnimationDocumentSlots();
}
