#include "StageResource.hpp"
#include "AnimationDocument.hpp"
#include <stddef.h>
#include <new>

// Both owners operate on the native 32-slot animation document table inside
// the renderer. The names are reconstruction labels, not claimed original
// C++ identifiers.
typedef char DocumentSlotsOffsetCheck[
    (offsetof(AnimationDocumentSlotsView, files) == 0x4350c0) ? 1 : -1];
typedef char DocumentPendingDestroyOffsetCheck[
    (offsetof(AnimationDocumentView, pendingDestroy) == 0x128) ? 1 : -1];
typedef char DocumentBusyOffsetCheck[
    (offsetof(AnimationDocumentView, busy) == 0x124) ? 1 : -1];

// TH11 004549A0..004549D2 (51 bytes): the caller supplies the renderer
// and a bounded slot. The original private /GL argument ABI uses EBX/ESI.
void __stdcall FreeAnimationDocumentSlot(RenderBatchView *renderer,
                                         unsigned int index)
{
    if (index >= 32)
        return;

    AnimationDocumentView **slot =
        &reinterpret_cast<AnimationDocumentSlotsView *>(renderer)->files[index];
    AnimationDocumentView *file = *slot;
    if (file)
    {
        DestroyAnimationDocument(file);
        ::operator delete(*slot);
        *slot = 0;
    }
}

// TH11 004548D0..00454956 (135 bytes): process pending animation load
// completion/teardown. The original has one caller at 00429067.
int __cdecl UpdateAnimationDocumentSlots()
{
    AnimationDocumentSlotsView *slots =
        reinterpret_cast<AnimationDocumentSlotsView *>(g_RenderBatch);

    for (unsigned int i = 0; i < 32; ++i)
    {
        AnimationDocumentView **slot = &slots->files[i];
        AnimationDocumentView *file = *slot;
        if (!file)
            continue;

        if (file->pendingDestroy)
        {
            // The native inlined destruction path checks the slot bounds
            // independently, even though this loop uses 0..31.
            if (i < 32)
            {
                DestroyAnimationDocument(file);
                ::operator delete(*slot);
                *slot = 0;
            }
            // Preserve the original unchecked reload rather than inserting
            // a new guard. In native code the pointer may already be null.
            (*slot)->pendingDestroy = 0;
        }
        else if (file->busy)
        {
            return (AdvanceAnimationDocument(*slot) != 0) - 1;
        }
    }
    return 0;
}
