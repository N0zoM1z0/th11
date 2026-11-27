#include "SecondaryDebugHudLifetime.hpp"
#include "SecondaryDebugHudDialogue.hpp"
#include "SecondaryActorRuntime.hpp"
#include "VmBindings.hpp"
#include "AnimationDocument.hpp"
#include "MenuController.hpp"
#include "MainRuntime.hpp"
#include <new>
#include <stdlib.h>

// The 0x4C3238 slot is the existing renderer/HUD prepared-data owner,
// not a second allocation made by this translation unit.
extern void *g_SecondaryHudPreparedData;

namespace
{
__forceinline void UnbindTrackedHudVm(unsigned int &id)
{
    if (id)
    {
        BoundVmView *vm = FindBoundVm(id);
        if (vm)
        {
            vm->flags |= 0x04000000u;
            if (!vm->marker018)
            {
                for (BoundVmNodeView *node = vm->related; node; node = node->next)
                    node->vm->flags |= 0x04000000u;
            }
        }
    }
    id = 0;
}
}

// Complete native 0x41AD30..0x41AF03. The source intentionally uses the
// original renderer and HUD singleton storage, rather than allocating a
// shadow cache or replacing real component destruction with fake bytecode.
void __stdcall PrepareSecondaryDebugHudDestruction(SecondaryDebugHudOwned *self)
{
    RenderBatchView *renderer = g_RenderBatch;
    if (!(g_StageResourceFlags & 9u))
    {
        AnimationDocumentSlotsView *slots =
            reinterpret_cast<AnimationDocumentSlotsView *>(renderer);
        AnimationDocumentView **documentSlot = &slots->files[27];
        AnimationDocumentView *document = *documentSlot;
        if (document)
        {
            DestroyAnimationDocument(document);
            ::operator delete(*documentSlot);
            *documentSlot = 0;
        }
    }
    else
    {
        MarkMenuFileAnimations(renderer, self->retainedAnimationFile);
    }

    self->retainedAnimationFile = 0;
    SecondaryHudDialogueOwnedView *dialogue =
        reinterpret_cast<SecondaryHudDialogueOwnedView *>(self->dialogue);
    if (dialogue)
    {
        DestroySecondaryHudDialogue(dialogue);
        ::operator delete(dialogue);
        self->dialogue = 0;
    }

    if (!(g_StageResourceFlags & 9u))
    {
        if (self->preparedData)
        {
            free(self->preparedData);
            self->preparedData = 0;
        }
        self->preparedData = 0;
        g_SecondaryHudPreparedData = 0;
    }
    else
        g_SecondaryHudPreparedData = self->preparedData;

    if (self->updateChain)
        self->updateChain->flags &= ~2u;

    AnimationVmId *ids = reinterpret_cast<AnimationVmId *>(self->precleanupAnimationIds);
    ReleaseSecondaryAnimation(ids[0]);
    ids[0] = 0;
    ReleaseSecondaryAnimation(ids[1]);
    ids[1] = 0;
    ReleaseSecondaryAnimation(*reinterpret_cast<AnimationVmId *>(self->precleanupAdditionalIds));

    for (int i = 0; i < 11; ++i)
        self->precleanupAdditionalIds[i] = 0;

    unsigned int *id = self->trackedVmIds;
    int remaining = 8;
    do
    {
        UnbindTrackedHudVm(*id);
        ++id;
    } while (--remaining);

    self->hudStateFlags |= 0x1c0u;
    self->reset43fc = 0;
}
