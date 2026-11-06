#include "MenuPrimary.hpp"
#include "AnimationVmFactory.hpp"
#include "AsciiLifecycle.hpp"
#include "MeshEffects.hpp"
#include "SecondaryActorRuntime.hpp"
#include "StageTween.hpp"
#include "VmBindings.hpp"

namespace
{
__forceinline void Select(SecondaryDebugSelectionView *selection, int value)
{
    if (selection->count)
        selection->current = selection->count <= value ?
            static_cast<int>(static_cast<unsigned int>(selection->count) - 1) : value;
    else selection->current = value;
}
__forceinline void MarkLockedEntries(MenuControllerView *self)
{
    if (!IsMenuExtraUnlocked())
    {
        SignalLockedMenuEntry(self, 4);
        SignalLockedMenuEntry(self, 12);
    }
}
__forceinline void SignalRoot(AnimationVmId id, short event)
{
    AnimationVmView *vm = FindAnimationVmById(g_RenderBatch, id);
    if (vm)
    {
        reinterpret_cast<SecondaryActorAnimationView *>(vm)->event = event;
        BoundVmView *bound = reinterpret_cast<BoundVmView *>(vm);
        if (!bound->marker018)
            for (BoundVmNodeView *node = bound->related; node; node = node->next)
                reinterpret_cast<SecondaryActorAnimationView *>(node->vm)->event = event;
    }
}
__forceinline void UpdateSelectionAnimation(MenuControllerView *self, int eventBase)
{
    SignalAndExecuteConfigurationAnimation(self->page.primaryVmId, 3);
    SignalSecondaryAnimationEvent(self->page.primaryVmId,
        static_cast<short>(self->page.selection.current + eventBase));
    MarkLockedEntries(self);
}
__forceinline void ClosePrimaryAnimations(MenuControllerView *self)
{
    StopMenuAnimation(self, 0x53);
    StopMenuAnimation(self, 0x54);
    SignalSecondaryAnimation(&self->secondaryRootVmId.value);
    SetConfigurationMenuState(&self->page, 4);
}
}

int __stdcall UpdateMenuPage01(MenuControllerView *self)
{
    if (static_cast<unsigned int>(self->page.state) > 4) return 1;
    int closingSound;
    switch (self->page.state)
    {
    case 0:
        self->page.selection.count = 8;
        if (!IsMenuExtraUnlocked())
        {
            int *excluded = reinterpret_cast<int *>(
                reinterpret_cast<unsigned char *>(&self->page.selection) + 0x90);
            excluded[self->page.selection.excludedCount] = 1;
            self->page.selection.excludedCount = static_cast<int>(
                static_cast<unsigned int>(self->page.selection.excludedCount) + 1);
        }
        if (g_StageResourceFlags & 0x10)
        {
            Select(&self->page.selection, 2);
            g_StageResourceFlags &= ~0x10u;
        }
        SetConfigurationMenuState(&self->page, 1);
        if (self->flags & 2)
        {
            StartMenuAnimation(self, 0x4f);
            StartMenuAnimation(self, 0x53);
            self->flags &= ~2u;
        }
        else
        {
            RenderBatchView *renderer = g_RenderBatch;
            if (!FindAnimationVmById(renderer, MenuAnimationId(self, 0x4f)) &&
                !FindAnimationVmById(renderer, MenuAnimationId(self, 0x50)))
            {
                StartMenuAnimation(self, 0x50);
                renderer = g_RenderBatch;
            }
            if (!FindAnimationVmById(renderer, MenuAnimationId(self, 0x53)) &&
                !FindAnimationVmById(renderer, MenuAnimationId(self, 0x54)))
                StartMenuAnimation(self, 0x54);
            SetStageTimer(&self->page.time, 70);
        }
        // Fall through to the opening animation.
    case 1:
        if (self->page.time.current == 70)
        {
            self->page.primaryVmId = CreateAnimationVmId(self->page.file, 0, 0x16);
            if (!FindAnimationVmById(g_RenderBatch, self->secondaryRootVmId))
            {
                self->secondaryRootVmId = AnimationVmId(0);
                self->secondaryRootVmId = CreateAnimationVmId(self->page.secondaryFile, 0, 0x16);
            }
            MarkLockedEntries(self);
        }
        if (self->page.time.current > 80)
        {
            SetConfigurationMenuState(&self->page, 2);
            UpdateSelectionAnimation(self, 0x11);
        }
        break;
    case 2:
    {
        SecondaryDebugSelectionView *selection = &self->page.selection;
        selection->previous = selection->current;
        if ((g_InputState.pressed & 0x10) || (g_InputState.repeat & 0x10))
            MoveSecondaryDebugSelection(selection, -1);
        if ((g_InputState.pressed & 0x20) || (g_InputState.repeat & 0x20))
            MoveSecondaryDebugSelection(selection, 1);
        if (selection->previous != selection->current)
        {
            QueueMeshEffectSound(12);
            UpdateSelectionAnimation(self, 7);
        }
        if (g_InputState.pressed & 0x102)
        {
            if (selection->current == 7)
            {
                goto Quit;
            }
            QueueMeshEffectSound(11);
            Select(selection, 7);
            UpdateSelectionAnimation(self, 7);
        }
        if (g_InputState.pressed & 0x80001)
        {
            SignalRoot(self->page.primaryVmId, 6);
            switch (selection->current)
            {
            case 0: case 1: case 2: case 3: case 4:
                QueueMeshEffectSound(10);
                ClosePrimaryAnimations(self);
                return 1;
            case 5:
                QueueMeshEffectSound(10);
                ClosePrimaryAnimations(self);
                QueueMeshEffectSound(10);
                return 1;
            case 6:
                closingSound = 10;
                goto FinishClose;
            case 7:
            Quit:
                closingSound = 11;
            FinishClose:
                QueueMeshEffectSound(closingSound);
                SetConfigurationMenuState(&self->page, 4);
                return 1;
            }
        }
        break;
    }
    case 3: break;
    case 4:
    {
        if (self->page.time.current < 20) break;
        SecondaryDebugSelectionView *selection = &self->page.selection;
        switch (selection->current)
        {
        case 0:
            g_StageResourceFlags &= ~0x10u;
            goto DifficultyPage;
        case 1:
            g_StageResourceFlags &= ~0x10u;
            StopMenuAnimation(self, 0x4f);
            SetConfigurationMenuPage(&self->page, 5);
            PushConfigurationSelection(selection);
            *reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(self) + 0x58e8) =
                g_SecondaryActorDifficulty;
            g_MenuPreferredDifficulty = 4;
            g_SecondaryActorDifficulty = 4;
            Select(selection, 0);
            return 1;
        case 2:
            g_StageResourceFlags |= 0x10;
        DifficultyPage:
        {
            StopMenuAnimation(self, 0x4f);
            SetConfigurationMenuPage(&self->page, 5);
            PushConfigurationSelection(selection);
            int difficulty = g_MenuPreferredDifficulty;
            if (difficulty >= 4)
            {
                difficulty = 1;
                g_MenuPreferredDifficulty = difficulty;
                g_SecondaryActorDifficulty = difficulty;
            }
            SelectMenuItem(difficulty, selection);
            g_SecondaryActorDifficulty = difficulty;
            return 1;
        }
        case 3:
            StopMenuAnimation(self, 0x4f);
            SetConfigurationMenuPage(&self->page, 11);
            PushConfigurationSelection(selection);
            return 1;
        case 4:
            StopMenuAnimation(self, 0x4f);
            SetConfigurationMenuPage(&self->page, 10);
            PushConfigurationSelection(selection);
            return 1;
        case 5:
            StopMenuAnimation(self, 0x4f);
            SetConfigurationMenuPage(&self->page, 13);
            PushConfigurationSelection(selection);
            return 1;
        case 6:
            SetConfigurationMenuPage(&self->page, 3);
            PushConfigurationSelection(selection);
            return 1;
        case 7:
            SetConfigurationMenuPage(&self->page, 2);
            break;
        }
        break;
    }
    }
    return 1;
}
