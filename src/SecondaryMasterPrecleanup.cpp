#include "SecondaryActorState.hpp"
#include "VmBindings.hpp"
#include "AnimationDocument.hpp"
#include "Chain.hpp"
#include <stddef.h>

// 0040D0C0..0040D188: pre-destructor teardown for the secondary master.
// The renderer owns the four document slot references. Neither the VM lists
// nor the actor linked list gets deleted here; their owners handle that.
void __stdcall PrepareSecondaryMasterDestruction(SecondaryMasterView *self)
{
    BoundVmListsView *lists = reinterpret_cast<BoundVmListsView *>(g_RenderBatch);
    AnimationDocumentSlotsView *slots = reinterpret_cast<AnimationDocumentSlotsView *>(g_RenderBatch);
    AnimationDocumentView **slot = &slots->files[8];
    int remaining = 4;
    do
    {
        AnimationDocumentView *file = *slot;
        {
        BoundVmNodeView *node = lists->first;
        while (node)
        {
            BoundVmNodeView *next = node->next;
            AnimationVmView *vm = reinterpret_cast<AnimationVmView *>(node->vm);
            if (reinterpret_cast<void *>(vm->file) == file)
                vm->flags.value |= 0x04000000;
            node = next;
        }
        }
        {
        BoundVmNodeView *node = lists->second;
        while (node)
        {
            BoundVmNodeView *next = node->next;
            AnimationVmView *vm = reinterpret_cast<AnimationVmView *>(node->vm);
            if (reinterpret_cast<void *>(vm->file) == file)
                vm->flags.value |= 0x04000000;
            node = next;
        }
        }
        ++slot;
        --remaining;
    } while (remaining);

    for (SecondaryActorNode *node = self->first; node; )
    {
        SecondaryActorNode *next = node->next;
        SecondaryActorManagerView *actor = node->actor;
        if (actor) delete actor;
        node = next;
    }
    self->unknown074 = 0;
    self->Slot(0) = 0;
    ChainElement *update = *reinterpret_cast<ChainElement **>(reinterpret_cast<char *>(self) + 8);
    if (update) update->flags &= ~2u;
    ChainElement *draw = *reinterpret_cast<ChainElement **>(reinterpret_cast<char *>(self) + 12);
    if (draw) draw->flags &= ~2u;
}
