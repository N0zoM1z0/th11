#include "MenuPartnerSelection.hpp"
#include "MenuPracticeStage.hpp"
#include "MenuPrimary.hpp"
#include "AnimationVmFactory.hpp"
#include "MainRuntime.hpp"
#include "MeshEffects.hpp"
#include "SecondaryActorRuntime.hpp"
#include "SecondaryDialogue.hpp"
#include "SecondaryHud.hpp"
#include "VmBindings.hpp"

namespace
{
__forceinline void Select(SecondaryDebugSelectionView *selection, int value)
{
    if (selection->count)
        selection->current = selection->count <= value ? selection->count - 1 : value;
    else selection->current = value;
}

__forceinline void Exclude(SecondaryDebugSelectionView *selection, int value)
{
    int *excluded = reinterpret_cast<int *>(
        reinterpret_cast<unsigned char *>(selection) + 0x90);
    excluded[selection->excludedCount++] = value;
}

__forceinline void Create(MenuControllerView *self, int script)
{
    MenuAnimationId(self, script) = CreateAnimationVmId(self->page.file, script, 0x16);
}

__forceinline AnimationVmId Child(MenuControllerView *self, int script)
{
    return FindSecondaryDialogueChild(
        &MenuAnimationId(self, g_MeshEffectSelection0 + 0x8e).value, script);
}

__forceinline void HideUnavailableRecord(MenuControllerView *self, int offset, int slot)
{
    int *counts = reinterpret_cast<int *>(g_SecondaryGameplayStatistics + offset);
    if (!counts[g_MeshEffectSelection0 * 0x4e9f + g_SecondaryActorDifficulty])
        HideMenuAnimationTree(Child(self, g_MeshEffectSelection0 * 3 + 0x88 + slot));
}

__forceinline void SavePartner(SecondaryDebugSelectionView *selection)
{
    g_MeshEffectSelection1 = selection->current;
    g_MenuSavedPartnerSelection = selection->current;
}
}

int __stdcall UpdateMenuPage07(MenuControllerView *self)
{
    if (static_cast<unsigned int>(self->page.state) > 4) return 1;
    switch (self->page.state)
    {
    case 0:
    {
        self->page.selection.count = 3;
        if (g_SecondaryActorDifficulty == 4)
        {
            unsigned char *statistics = g_SecondaryGameplayStatistics;
            if (!(statistics[g_MeshEffectSelection0 * 3 + 0x2ddf0] & 0x10))
            {
                if (self->page.selection.current == 0)
                {
                    if (statistics[g_MeshEffectSelection0 * 3 + 0x2ddf1] & 0x10)
                        Select(&self->page.selection, 1);
                    else Select(&self->page.selection, 2);
                }
                Exclude(&self->page.selection, 0);
            }
            if (!(statistics[g_MeshEffectSelection0 * 3 + 0x2ddf1] & 0x10))
            {
                if (self->page.selection.current == 1)
                {
                    if (statistics[g_MeshEffectSelection0 * 3 + 0x2ddf0] & 0x10)
                        Select(&self->page.selection, 0);
                    else Select(&self->page.selection, 2);
                }
                Exclude(&self->page.selection, 1);
            }
            if (!(statistics[g_MeshEffectSelection0 * 3 + 0x2ddf2] & 0x10))
            {
                if (self->page.selection.current == 2)
                {
                    if (statistics[g_MeshEffectSelection0 * 3 + 0x2ddf0] & 0x10)
                        Select(&self->page.selection, 0);
                    else Select(&self->page.selection, 1);
                }
                Exclude(&self->page.selection, 2);
            }
        }
        Create(self, 0x62);
        StopMenuAnimation(self, g_MeshEffectSelection0 + 0x8e);
        Create(self, g_MeshEffectSelection0 + 0x8e);
        SignalAndExecuteConfigurationAnimation(
            MenuAnimationId(self, g_MeshEffectSelection0 + 0x8e), 3);
        SignalSecondaryAnimationEvent(MenuAnimationId(self, g_MeshEffectSelection0 + 0x8e),
            static_cast<short>(self->page.selection.current + 0x11));
        SetConfigurationMenuState(&self->page, 1);
        HideUnavailableRecord(self, 0x598, 0);
        HideUnavailableRecord(self, 0x6e6c, 1);
        HideUnavailableRecord(self, 0xd740, 2);
        // Fall through to the opening delay.
    }
    case 1:
        if (self->page.time.current > 6)
        {
            SetConfigurationMenuState(&self->page, 2);
            return 1;
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
            SignalAndExecuteConfigurationAnimation(
                MenuAnimationId(self, g_MeshEffectSelection0 + 0x8e), 3);
            SignalSecondaryAnimationEvent(MenuAnimationId(self, g_MeshEffectSelection0 + 0x8e),
                static_cast<short>(selection->current + 7));
        }
        if (g_InputState.pressed & 0x102)
        {
            SetConfigurationMenuState(&self->page, 4);
            QueueMeshEffectSound(11);
            return 1;
        }
        if (g_InputState.pressed & 0x80001)
        {
            SetConfigurationMenuState(&self->page, 3);
            QueueMeshEffectSound(10);
            AnimationVmView *vm = FindAnimationVmById(g_RenderBatch,
                Child(self, selection->current + g_MeshEffectSelection0 * 6 + 0x7c));
            if (vm)
            {
                reinterpret_cast<SecondaryActorAnimationView *>(vm)->event = 6;
                BoundVmView *bound = reinterpret_cast<BoundVmView *>(vm);
                if (!bound->marker018)
                    for (BoundVmNodeView *node = bound->related; node; node = node->next)
                        reinterpret_cast<SecondaryActorAnimationView *>(node->vm)->event = 6;
            }
        }
        break;
    }
    case 3:
        if (self->page.time.current == 10)
        {
            if (!(g_StageResourceFlags & 0x10))
            {
                StartMenuTransitionOverlay(480.0f, 392.0f);
                CreateSecondaryHudScreenEffect(5, 32, 0, 0, 61);
            }
            else
            {
                SavePartner(&self->page.selection);
                PushConfigurationSelection(&self->page.selection);
                StopMenuAnimation(self, 0x62);
                SetConfigurationMenuPage(&self->page, 8);
            }
        }
        if (self->page.time.current >= 40)
        {
            SavePartner(&self->page.selection);
            PushConfigurationSelection(&self->page.selection);
            if (!(g_StageResourceFlags & 0x10))
            {
                SetConfigurationMenuPage(&self->page, 2);
                if (g_SecondaryActorDifficulty < 4)
                {
                    g_SecondaryGameplaySelectionEC8 = reinterpret_cast<SecondaryGameplaySelectionView *>(
                        g_MenuStageSelections + 0x40);
                    g_StageVariant = 1;
                    g_MenuPreviousVariant = 1;
                }
                else
                {
                    g_SecondaryGameplaySelectionEC8 = reinterpret_cast<SecondaryGameplaySelectionView *>(
                        g_MenuStageSelections + 0x1c0);
                    g_StageVariant = 7;
                    g_MenuPreviousVariant = 7;
                }
                *reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(&g_MainRuntime) + 0x558) = 7;
                EnqueueMenuMusicFade(6.0f);
                return 1;
            }
            StopMenuAnimation(self, 0x62);
            SetConfigurationMenuPage(&self->page, 8);
            return 1;
        }
        break;
    case 4:
        if (self->page.time.current >= 6)
        {
            SavePartner(&self->page.selection);
            StopMenuAnimation(self, g_MeshEffectSelection0 + 0x8e);
            StopMenuAnimation(self, 0x62);
            SetConfigurationMenuPage(&self->page, 6);
            PopConfigurationSelection(&self->page.selection);
        }
        break;
    }
    return 1;
}
