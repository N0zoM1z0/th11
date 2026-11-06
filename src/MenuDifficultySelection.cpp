#include "MenuDifficultySelection.hpp"
#include "MenuCharacterSelection.hpp"
#include "MenuPrimary.hpp"
#include "AnimationVmFactory.hpp"
#include "AsciiLifecycle.hpp"
#include "MeshEffects.hpp"
#include "SecondaryActorRuntime.hpp"
#include "SecondaryDialogue.hpp"
#include "VmBindings.hpp"

namespace
{
__forceinline void Create(MenuControllerView *self, int script)
{
    MenuAnimationId(self, script) = CreateAnimationVmId(self->page.file, script, 0x16);
}

__forceinline void Signal(AnimationVmView *vm, short event)
{
    if (vm)
    {
        reinterpret_cast<SecondaryActorAnimationView *>(vm)->event = event;
        BoundVmView *bound = reinterpret_cast<BoundVmView *>(vm);
        if (!bound->marker018)
            for (BoundVmNodeView *node = bound->related; node; node = node->next)
                reinterpret_cast<SecondaryActorAnimationView *>(node->vm)->event = event;
    }
}

__forceinline AnimationVmId ExtraChild(RenderBatchView *renderer, AnimationVmId *parent)
{
    AnimationVmView *vm = FindAnimationVmById(renderer, *parent);
    if (!vm) *parent = AnimationVmId(0);
    for (AnimationVmNode *node = &vm->drawNode; node; node = node->next)
        if (static_cast<short>(node->vm->scriptId) == 0x6f)
            return AnimationVmId(node->vm->unknown000);
    return AnimationVmId(0);
}
}

int __stdcall UpdateMenuPage05(MenuControllerView *self)
{
    int script = 0x75 + (g_SecondaryActorDifficulty >= 4);
    if (static_cast<unsigned int>(self->page.state) > 4) return 1;
    switch (self->page.state)
    {
    case 0:
        if (!FindAnimationVmById(g_RenderBatch, MenuAnimationId(self, 0x5c)))
        {
            StartMenuAnimation(self, 0x5c);
            MenuDifficultyFooterId(self) = CreateAnimationVmId(
                reinterpret_cast<AnimationFileView *>(g_AsciiManager->animation), 0x12, 0x16);
        }
        self->page.selection.count = g_MenuPreferredDifficulty < 4 ? 4 : 1;
        g_SecondaryActorDifficulty = g_MenuPreferredDifficulty;
        StopMenuAnimation(self, script);
        Create(self, script);
        SignalAndExecuteConfigurationAnimation(MenuAnimationId(self, script), 3);
        SignalSecondaryAnimationEvent(MenuAnimationId(self, script),
            static_cast<short>(self->page.selection.current + 0x11));
        Create(self, 0x60);
        SetConfigurationMenuState(&self->page, 1);
        // Fall through to the opening delay.
    case 1:
        if (self->page.time.current > 6)
        {
            SetConfigurationMenuState(&self->page, 2);
            return 1;
        }
        break;
    case 2:
        if (g_SecondaryActorDifficulty < 4)
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
                ExecuteMenuAnimationEvent3(self, script);
                SignalSecondaryAnimationEvent(MenuAnimationId(self, script),
                    static_cast<short>(selection->current + 7));
            }
        }
        if (g_InputState.pressed & 0x102)
        {
            SetConfigurationMenuState(&self->page, 4);
            QueueMeshEffectSound(11);
            StopMenuAnimation(self, script);
            return 1;
        }
        if (g_InputState.pressed & 0x80001)
        {
            AnimationVmId *root = &MenuAnimationId(self, script);
            RenderBatchView *renderer = g_RenderBatch;
            Signal(FindAnimationVmById(renderer, *root), 6);
            if (g_SecondaryActorDifficulty < 4)
            {
                AnimationVmId child = FindSecondaryDialogueChild(
                    &root->value, self->page.selection.current + 0x6b);
                Signal(FindAnimationVmById(g_RenderBatch, child), 2);
            }
            else
            {
                AnimationVmId child = ExtraChild(renderer, root);
                Signal(FindAnimationVmById(renderer, child), 2);
            }
            SetConfigurationMenuState(&self->page, 3);
            QueueMeshEffectSound(10);
            return 1;
        }
        break;
    case 3:
        if (self->page.time.current >= 14)
        {
            StopMenuAnimation(self, 0x60);
            SetConfigurationMenuPage(&self->page, 6);
            if (g_SecondaryActorDifficulty < 4)
                g_SecondaryActorDifficulty = self->page.selection.current;
            g_MenuPreferredDifficulty = g_SecondaryActorDifficulty;
            SecondaryDebugSelectionView *selection = &self->page.selection;
            PushConfigurationSelection(selection);
            int character = g_MenuSavedCharacterSelection;
            self->page.selection.wrapEnabled = 1;
            self->page.selection.count = 2;
            SelectMenuItem(character, selection);
            g_MeshEffectSelection0 = character;
            return 1;
        }
        break;
    case 4:
        if (self->page.time.current >= 6)
        {
            StopMenuAnimation(self, 0x60);
            StopMenuAnimation(self, 0x5c);
            SignalSecondaryAnimation(&MenuDifficultyFooterId(self).value);
            SetConfigurationMenuPage(&self->page, 1);
            int difficulty;
            if (g_SecondaryActorDifficulty < 4) difficulty = self->page.selection.current;
            else difficulty = *reinterpret_cast<int *>(
                reinterpret_cast<unsigned char *>(self) + 0x58e8);
            g_SecondaryActorDifficulty = difficulty;
            g_MenuPreferredDifficulty = difficulty;
            PopConfigurationSelection(&self->page.selection);
        }
        break;
    }
    return 1;
}
