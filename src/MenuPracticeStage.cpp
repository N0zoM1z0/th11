#include "MenuPracticeStage.hpp"
#include "MenuPlayerData.hpp"
#include "MenuPrimary.hpp"
#include "AnimationVmFactory.hpp"
#include "MainRuntime.hpp"
#include "MeshEffects.hpp"
#include "SecondaryActorRuntime.hpp"
#include "SecondaryHud.hpp"

namespace
{
__forceinline int RestoredStage(SecondaryDebugSelectionView *selection)
{
    if (selection->count)
    {
        int value = g_MenuPracticeStageSelection;
        return selection->count <= value ? selection->count - 1
            : value < 0 ? 0 : value;
    }
    return g_MenuPracticeStageSelection;
}
__forceinline void Create(MenuControllerView *self, int script)
{
    MenuAnimationId(self, script) = CreateAnimationVmId(self->page.file, script, 0x16);
}
}

int __stdcall UpdateMenuPage08(MenuControllerView *self)
{
    MenuPracticeStageView *stage = reinterpret_cast<MenuPracticeStageView *>(self);
    if (static_cast<unsigned int>(self->page.state) <= 4)
    switch (self->page.state)
    {
    case 0:
        stage->selection.count = 6;
        stage->selection.current = RestoredStage(&stage->selection);
        Create(self, 0x68);
        Create(self, g_MeshEffectSelection0 + 0x69);
        SetConfigurationMenuState(&self->page, 1);
        // Fall through to the opening animation.
    case 1:
        if (self->page.time.current > 10)
        {
            SetConfigurationMenuState(&self->page, 2);
            return 1;
        }
        break;
    case 2:
    {
        SecondaryDebugSelectionView *selection = &stage->selection;
        selection->previous = selection->current;
        if ((g_InputState.pressed & 0x10) || (g_InputState.repeat & 0x10))
            MoveSecondaryDebugSelection(selection, -1);
        if ((g_InputState.pressed & 0x20) || (g_InputState.repeat & 0x20))
            MoveSecondaryDebugSelection(selection, 1);
        if (selection->previous != selection->current) QueueMeshEffectSound(12);
        if (g_InputState.pressed & 0x102)
        {
            SetConfigurationMenuState(&self->page, 4);
            QueueMeshEffectSound(11);
            g_MenuPracticeStageSelection = selection->current;
            return 1;
        }
        if (g_InputState.pressed & 0x80001)
        {
            int group = g_MeshEffectSelection1 + g_MeshEffectSelection0 * 3;
            int index = selection->current + g_SecondaryActorDifficulty * 6;
            int sound;
            if (!g_SecondaryGameplayStatistics[group * 0x68d4 + 0x5b1 + index * 8])
                sound = 37;
            else
            {
                SetConfigurationMenuState(&self->page, 3);
                sound = 10;
            }
            QueueMeshEffectSound(sound);
            g_MenuPracticeStageSelection = selection->current;
            g_MenuStageShortcut = 0;
            int kind = ReadMenuKeyboardSnapshot(g_MenuStageShortcutKeys);
            // This caller tests zero/nonzero, unlike the player-data caller's
            // explicit kind2 mapping. Preserve the observed digit-key choice.
            if (!kind)
            {
                if (g_MenuStageShortcutKeys['1'] & 0x80) { g_MenuStageShortcut = 1; return 1; }
                if (g_MenuStageShortcutKeys['2'] & 0x80) { g_MenuStageShortcut = 2; return 1; }
                if (g_MenuStageShortcutKeys['3'] & 0x80) { g_MenuStageShortcut = 3; return 1; }
                if (g_MenuStageShortcutKeys['4'] & 0x80) { g_MenuStageShortcut = 4; return 1; }
                if (g_MenuStageShortcutKeys['5'] & 0x80) { g_MenuStageShortcut = 5; return 1; }
                if (g_MenuStageShortcutKeys['6'] & 0x80) { g_MenuStageShortcut = 6; return 1; }
                if (g_MenuStageShortcutKeys['7'] & 0x80) { g_MenuStageShortcut = 7; return 1; }
                if (g_MenuStageShortcutKeys['8'] & 0x80) { g_MenuStageShortcut = 8; return 1; }
                if (g_MenuStageShortcutKeys['9'] & 0x80) { g_MenuStageShortcut = 9; return 1; }
            }
            else
            {
                if (g_MenuStageShortcutKeys[0x02] & 0x80) { g_MenuStageShortcut = 1; return 1; }
                if (g_MenuStageShortcutKeys[0x03] & 0x80) { g_MenuStageShortcut = 2; return 1; }
                if (g_MenuStageShortcutKeys[0x04] & 0x80) { g_MenuStageShortcut = 3; return 1; }
                if (g_MenuStageShortcutKeys[0x05] & 0x80) { g_MenuStageShortcut = 4; return 1; }
                if (g_MenuStageShortcutKeys[0x06] & 0x80) { g_MenuStageShortcut = 5; return 1; }
                if (g_MenuStageShortcutKeys[0x07] & 0x80) { g_MenuStageShortcut = 6; return 1; }
                if (g_MenuStageShortcutKeys[0x08] & 0x80) { g_MenuStageShortcut = 7; return 1; }
                if (g_MenuStageShortcutKeys[0x09] & 0x80) { g_MenuStageShortcut = 8; return 1; }
                if (g_MenuStageShortcutKeys[0x0a] & 0x80) { g_MenuStageShortcut = 9; return 1; }
            }
        }
        break;
    }
    case 3:
        if (self->page.time.current == 10)
        {
            StartMenuTransitionOverlay(480.0f, 392.0f);
            CreateSecondaryHudScreenEffect(5, 32, 0, 0, 57);
        }
        if (self->page.time.current >= 40)
        {
            PushConfigurationSelection(&stage->selection);
            SetConfigurationMenuPage(&self->page, 2);
            int variant = stage->selection.current + 1;
            g_SecondaryGameplaySelectionEC8 = reinterpret_cast<SecondaryGameplaySelectionView *>(
                g_MenuStageSelections + variant * 0x40);
            g_StageVariant = variant;
            g_MenuPreviousVariant = variant;
            EnqueueMenuMusicFade(6.0f);
            *reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(&g_MainRuntime) + 0x558) = 7;
            return 1;
        }
        break;
    case 4:
        if (self->page.time.current >= 6)
        {
            StopMenuAnimation(self, 0x68);
            StopMenuAnimation(self, g_MeshEffectSelection0 + 0x69);
            SetConfigurationMenuPage(&self->page, 7);
            PopConfigurationSelection(&stage->selection);
        }
        break;
    }
    return 1;
}
