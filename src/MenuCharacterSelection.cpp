#include "MenuCharacterSelection.hpp"
#include "MenuPartnerSelection.hpp"
#include "MenuPrimary.hpp"
#include "AnimationVmFactory.hpp"
#include "MeshEffects.hpp"
#include "SecondaryActorRuntime.hpp"
#include "SecondaryDialogue.hpp"
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

__forceinline void SignalChild(MenuControllerView *self, int script, short event)
{
    AnimationVmId id = FindSecondaryDialogueChild(&MenuAnimationId(self, 0x7b).value, script);
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
}

int __stdcall UpdateMenuPage06(MenuControllerView *self)
{
    if (static_cast<unsigned int>(self->page.state) > 4) return 1;
    switch (self->page.state)
    {
    case 0:
    {
        self->page.selection.count = 2;
        if (g_SecondaryActorDifficulty == 4)
        {
            unsigned char *statistics = g_SecondaryGameplayStatistics;
            if (!(statistics[0x2ddf0] & 0x10) && !(statistics[0x2ddf1] & 0x10) &&
                !(statistics[0x2ddf2] & 0x10))
            {
                if (self->page.selection.current == 0) Select(&self->page.selection, 1);
                Exclude(&self->page.selection, 0);
            }
            if (!(statistics[0x2ddf3] & 0x10) && !(statistics[0x2ddf4] & 0x10) &&
                !(statistics[0x2ddf5] & 0x10))
            {
                if (self->page.selection.current == 1) Select(&self->page.selection, 0);
                Exclude(&self->page.selection, 1);
            }
        }
        Create(self, 0x61);
        StopMenuAnimation(self, 0x7b);
        Create(self, 0x7b);
        SignalAndExecuteConfigurationAnimation(MenuAnimationId(self, 0x7b), 3);
        SignalSecondaryAnimationEvent(MenuAnimationId(self, 0x7b),
            static_cast<short>(self->page.selection.current + 0x11));
        SetConfigurationMenuState(&self->page, 1);
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
        if ((g_InputState.pressed & 0x40) || (g_InputState.repeat & 0x40))
            MoveSecondaryDebugSelection(selection, -1);
        if ((g_InputState.pressed & 0x80) || (g_InputState.repeat & 0x80))
            MoveSecondaryDebugSelection(selection, 1);
        if (selection->previous != selection->current)
        {
            QueueMeshEffectSound(12);
            SignalAndExecuteConfigurationAnimation(MenuAnimationId(self, 0x7b), 3);
            SignalSecondaryAnimationEvent(MenuAnimationId(self, 0x7b),
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
            SignalChild(self, selection->current + 0x77, 6);
            SignalChild(self, selection->current + 0x79, 6);
            SignalChild(self, 0x78 - selection->current, 1);
            SignalChild(self, 0x7a - selection->current, 1);
            QueueMeshEffectSound(10);
            SetConfigurationMenuState(&self->page, 3);
            return 1;
        }
        break;
    }
    case 3:
        if (self->page.time.current >= 14)
        {
            StopMenuAnimation(self, 0x61);
            SetConfigurationMenuPage(&self->page, 7);
            SecondaryDebugSelectionView *selection = &self->page.selection;
            int current = selection->current;
            int previous = g_MeshEffectSelection0;
            g_MeshEffectSelection0 = current;
            g_MenuSavedCharacterSelection = current;
            PushConfigurationSelection(selection);
            self->page.selection.count = 3;
            if (previous != current)
            {
                Select(selection, 0);
                g_MenuSavedPartnerSelection = 0;
                return 1;
            }
            int partner = g_MenuSavedPartnerSelection;
            SelectMenuItem(partner, selection);
            g_MeshEffectSelection1 = partner;
            return 1;
        }
        break;
    case 4:
        if (self->page.time.current >= 6)
        {
            StopMenuAnimation(self, 0x7b);
            StopMenuAnimation(self, 0x61);
            SetConfigurationMenuPage(&self->page, 5);
            g_MeshEffectSelection0 = self->page.selection.current;
            PopConfigurationSelection(&self->page.selection);
            g_MenuSavedCharacterSelection = g_MeshEffectSelection0;
        }
        break;
    }
    return 1;
}
