#include "SecondaryDebugHudLifetime.hpp"
#include "SecondaryDebugHudDialogue.hpp"
#include "SecondaryActorRuntime.hpp"
#include "AnimationDocument.hpp"
#include <new>
#include <stdlib.h>

// 0x41AF30: the independent HUD resource-release path used by the
// scene owner. This does not unregister the three chain callbacks and
// must not be conflated with the larger 0x41AD30 destructor prepass.
void ClearSecondaryDebugHudResources()
{
    SecondaryDebugHudOwned *hud =
        reinterpret_cast<SecondaryDebugHudOwned *>(g_SecondaryActorHud);

    SecondaryHudDialogueOwnedView *dialogue =
        reinterpret_cast<SecondaryHudDialogueOwnedView *>(hud->dialogue);
    if (dialogue)
    {
        DestroySecondaryHudDialogue(dialogue);
        ::operator delete(dialogue);
        hud->dialogue = 0;
    }

    if (!(g_StageResourceFlags & 9u))
    {
        AnimationDocumentSlotsView *slots =
            reinterpret_cast<AnimationDocumentSlotsView *>(g_RenderBatch);
        AnimationDocumentView **documentSlot = &slots->files[27];
        AnimationDocumentView *document = *documentSlot;
        if (document)
        {
            DestroyAnimationDocument(document);
            ::operator delete(*documentSlot);
            *documentSlot = 0;
        }

        hud->retainedAnimationFile = 0;
        if (hud->preparedData)
        {
            free(hud->preparedData);
            hud->preparedData = 0;
        }
        hud->preparedData = 0;
    }
}
