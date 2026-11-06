#include "MenuPlayerData.hpp"
#include "MenuPrimary.hpp"
#include "AnimationVmFactory.hpp"
#include "AsciiLifecycle.hpp"
#include "MeshEffects.hpp"
#include "SecondaryActorRuntime.hpp"
#include <string.h>

namespace
{
__forceinline void Select(SecondaryDebugSelectionView *selection, int value)
{
    if (selection->count)
        selection->current = selection->count <= value ?
            static_cast<int>(static_cast<unsigned int>(selection->count) - 1) : value;
    else selection->current = value;
}
__forceinline void Create(MenuControllerView *self, int script)
{
    MenuAnimationId(self, script) = CreateAnimationVmId(self->page.file, script, 0x16);
}
__forceinline void SignalTree(AnimationVmView *vm)
{
    if (vm)
    {
        reinterpret_cast<SecondaryActorAnimationView *>(vm)->event = 1;
        BoundVmView *bound = reinterpret_cast<BoundVmView *>(vm);
        if (!bound->marker018)
            for (BoundVmNodeView *node = bound->related; node; node = node->next)
                reinterpret_cast<SecondaryActorAnimationView *>(node->vm)->event = 1;
    }
}
}

int __stdcall UpdateMenuPage0A(MenuControllerView *self)
{
    MenuPlayerDataView *data = reinterpret_cast<MenuPlayerDataView *>(self);
    switch (self->page.state)
    {
    case 0:
        data->character.count = 6;
        Select(&data->character, 0);
        data->difficulty.count = 5;
        Select(&data->difficulty, 1);
        data->difficulty.wrapEnabled = 1;
        data->recordPage.count = (CountMenuSpellsForDifficulty(data->difficulty.current) + 9) / 10 + 1;
        Select(&data->recordPage, 0);
        data->recordPage.wrapEnabled = 1;
        if (!FindAnimationVmById(g_RenderBatch, MenuAnimationId(self, 0x5c)))
        {
            StartMenuAnimation(self, 0x5c);
            MenuAnimationId(self, 0xc1) = CreateAnimationVmId(
                reinterpret_cast<AnimationFileView *>(g_AsciiManager->animation), 0x12, 0x16);
        }
        Create(self, 0x64);
        SetConfigurationMenuState(&self->page, 1);
        Create(self, data->character.current / 3 + 0x96);
        Create(self, data->character.current + 0x98);
        Create(self, data->difficulty.current + 0x9e);
        Create(self, 0xa6); Create(self, 0xa7);
        Create(self, 0xa8); Create(self, 0xa9);
        Create(self, 0xa3); Create(self, 0xa4); Create(self, 0xa5);
        Create(self, 0xaa);
        // Fall through to the opening animation.
    case 1:
        if (self->page.time.current > 6)
        {
            SetConfigurationMenuState(&self->page, 2);
            return 1;
        }
        break;
    case 2:
        data->character.previous = data->character.current;
        data->difficulty.previous = data->difficulty.current;
        data->recordPage.previous = data->recordPage.current;
        if ((g_InputState.pressed & 0x10) || (g_InputState.repeat & 0x10))
        {
            MoveSecondaryDebugSelection(&data->difficulty, -1);
            SignalAndExecuteConfigurationAnimation(MenuAnimationId(self, 0xa8), 2);
        }
        if ((g_InputState.pressed & 0x20) || (g_InputState.repeat & 0x20))
        {
            MoveSecondaryDebugSelection(&data->difficulty, 1);
            SignalAndExecuteConfigurationAnimation(MenuAnimationId(self, 0xa9), 2);
        }
        if (data->difficulty.previous != data->difficulty.current)
        {
            QueueMeshEffectSound(12);
            StopMenuAnimation(self, data->difficulty.previous + 0x9e);
            StartMenuAnimation(self, data->difficulty.current + 0x9e);
            if (data->recordPage.current > 0)
            {
                Select(&data->recordPage, 1);
                RefreshMenuPlayerDataRecords(self);
            }
            data->recordPage.count = (CountMenuSpellsForDifficulty(data->difficulty.current) + 9) / 10 + 1;
        }
        if ((g_InputState.pressed & 0x40) || (g_InputState.repeat & 0x40))
        {
            MoveSecondaryDebugSelection(&data->character, -1);
            SignalAndExecuteConfigurationAnimation(MenuAnimationId(self, 0xa6), 2);
        }
        if ((g_InputState.pressed & 0x80) || (g_InputState.repeat & 0x80))
        {
            MoveSecondaryDebugSelection(&data->character, 1);
            SignalAndExecuteConfigurationAnimation(MenuAnimationId(self, 0xa7), 2);
        }
        if (data->character.previous != data->character.current)
        {
            QueueMeshEffectSound(12);
            if (data->character.current / 3 != data->character.previous / 3)
            {
                StopMenuAnimation(self, data->character.previous / 3 + 0x96);
                StartMenuAnimation(self, data->character.current / 3 + 0x96);
            }
            StopMenuAnimation(self, data->character.previous + 0x98);
            StartMenuAnimation(self, data->character.current + 0x98);
            if (data->recordPage.current > 0) RefreshMenuPlayerDataRecords(self);
        }
        if (g_InputState.pressed & 0x80001)
        {
            if (data->recordPage.current == 0)
                for (int row = 0; row < 10; ++row)
                    data->rowVmIds[row] = CreateAnimationVmId(g_MenuMusicCommentFile, row + 0x19, 0x16);
            MoveSecondaryDebugSelection(&data->recordPage, 1);
            if (data->recordPage.current == 0)
                for (int row = 0; row < 10; ++row)
                    SignalSecondaryAnimation(&data->rowVmIds[row].value);
            else RefreshMenuPlayerDataRecords(self);
            QueueMeshEffectSound(10);
        }
        if (data->difficulty.current == 4 && data->character.current == 3)
        {
            if (g_InputState.pressed & 0x80103)
            {
                g_MenuPlayerDataSequencePosition = 0;
                g_MenuPlayerDataSequenceFrames = 0;
            }
            memcpy(g_MenuPlayerDataKeyboardPrevious, g_MenuPlayerDataKeyboardCurrent, 256);
            int inputKind = ReadMenuKeyboardSnapshot(g_MenuPlayerDataKeyboardCurrent);
            if (inputKind == 2)
            {
                unsigned char *keys = g_MenuPlayerDataKeyboardCurrent;
                unsigned char *mapped = g_MenuPlayerDataKeyboardPressed;
                memset(mapped, 0, 256);
                mapped[0x1e] = keys['A']; mapped[0x30] = keys['B'];
                mapped[0x2e] = keys['C']; mapped[0x20] = keys['D'];
                mapped[0x12] = keys['E']; mapped[0x21] = keys['F'];
                mapped[0x22] = keys['G']; mapped[0x23] = keys['H'];
                mapped[0x17] = keys['I']; mapped[0x24] = keys['J'];
                mapped[0x25] = keys['K']; mapped[0x26] = keys['L'];
                mapped[0x32] = keys['M']; mapped[0x31] = keys['N'];
                mapped[0x18] = keys['O']; mapped[0x19] = keys['P'];
                mapped[0x10] = keys['Q']; mapped[0x13] = keys['R'];
                mapped[0x1f] = keys['S']; mapped[0x14] = keys['T'];
                mapped[0x16] = keys['U']; mapped[0x2f] = keys['V'];
                mapped[0x11] = keys['W']; mapped[0x2d] = keys['X'];
                mapped[0x15] = keys['Y']; mapped[0x2c] = keys['Z'];
                memcpy(g_MenuPlayerDataKeyboardCurrent, mapped, 256);
            }
            if (inputKind == 1 || inputKind == 2)
            {
                for (int key = 0; key < 256; ++key)
                    g_MenuPlayerDataKeyboardPressed[key] =
                        (g_MenuPlayerDataKeyboardPrevious[key] ^ g_MenuPlayerDataKeyboardCurrent[key]) &
                        g_MenuPlayerDataKeyboardCurrent[key];
                if (g_MenuPlayerDataSequencePosition < 12)
                {
                    int expected = g_MenuPlayerDataSequenceKeys[g_MenuPlayerDataSequencePosition];
                    if (g_MenuPlayerDataKeyboardPressed[expected] & 0x80)
                    {
                        ++g_MenuPlayerDataSequencePosition;
                        g_MenuPlayerDataSequenceFrames = 0;
                    }
                    else
                    {
                        unsigned char any = 0;
                        for (int key = 0; key < 57; ++key) any |= g_MenuPlayerDataKeyboardPressed[key];
                        if (any & 0x80) g_MenuPlayerDataSequencePosition = 0;
                    }
                }
                else
                {
                    UnlockMenuPlayerDataEntries();
                    QueueMeshEffectSound(44);
                    g_MenuPlayerDataSequencePosition = 0;
                }
            }
            ++g_MenuPlayerDataSequenceFrames;
            if (g_MenuPlayerDataSequenceFrames > 300)
            {
                g_MenuPlayerDataSequencePosition = 0;
                g_MenuPlayerDataSequenceFrames = 0;
            }
        }
        if (g_InputState.pressed & 0x102)
        {
            SetConfigurationMenuState(&self->page, 3);
            QueueMeshEffectSound(11);
            StopMenuAnimation(self, data->difficulty.current + 0x9e);
            StopMenuAnimation(self, data->character.current + 0x98);
            StopMenuAnimation(self, data->character.current / 3 + 0x96);
            StopMenuAnimation(self, 0xa6); StopMenuAnimation(self, 0xa7);
            StopMenuAnimation(self, 0xa8); StopMenuAnimation(self, 0xa9);
            StopMenuAnimation(self, 0xa3); StopMenuAnimation(self, 0xa4);
            StopMenuAnimation(self, 0xa5); StopMenuAnimation(self, 0xaa);
            BoundVmListsView *lists = reinterpret_cast<BoundVmListsView *>(g_RenderBatch);
            for (int row = 0; row < 10; ++row)
                SignalTree(FindMenuPlayerDataVm(lists, data->rowVmIds[row].value));
        }
        break;
    case 3:
        if (self->page.time.current >= 6)
        {
            StopMenuAnimation(self, 0x64);
            StopMenuAnimation(self, 0x5c);
            SignalSecondaryAnimation(&MenuAnimationId(self, 0xc1).value);
            SetConfigurationMenuPage(&self->page, 1);
            PopConfigurationSelection(&data->character);
        }
        break;
    }
    return 1;
}
