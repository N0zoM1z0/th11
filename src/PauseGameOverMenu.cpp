#include "PauseContinueMenu.hpp"
#include "MenuPrimary.hpp"
#include "MenuPartnerSelection.hpp"
#include "SecondaryPlayerController.hpp"
#include "MeshEffects.hpp"
#include "VmBindings.hpp"
#include <stdio.h>
#include <string.h>
#include <time.h>

namespace
{
const char NameCharacters[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+-=.,!?@:;[]()_/{}|~^#$%&*   ";
struct ReplaySaveMetadata { unsigned char unknown000[0x68]; int stage; };

__forceinline void Choose(int value, SecondaryDebugSelectionView *selection)
{
    if (selection->count)
        selection->current = value >= selection->count ?
            static_cast<int>(static_cast<unsigned int>(selection->count) - 1) :
            value < 0 ? 0 : value;
    else selection->current = value;
}
__forceinline void Vertical(SecondaryDebugSelectionView *selection)
{
    selection->previous = selection->current;
    if ((g_InputState.pressed & 0x10) || (g_InputState.repeat & 0x10))
        MoveSecondaryDebugSelection(selection, -1);
    if ((g_InputState.pressed & 0x20) || (g_InputState.repeat & 0x20))
        MoveSecondaryDebugSelection(selection, 1);
}
__forceinline void Keyboard(SecondaryDebugSelectionView *selection)
{
    selection->previous = selection->current;
    if ((g_InputState.pressed & 0x10) || (g_InputState.repeat & 0x10))
        MoveSecondaryDebugSelection(selection, -13);
    if ((g_InputState.pressed & 0x20) || (g_InputState.repeat & 0x20))
        MoveSecondaryDebugSelection(selection, 13);
    if ((g_InputState.pressed & 0x40) || (g_InputState.repeat & 0x40))
    {
        int current = selection->current;
        // Keep VC9's native constant-multiply quotient codegen, not IDIV.
        int remainder = current - (current / 13) * 13;
        MoveSecondaryDebugSelection(selection, remainder == 0 ? 12 : -1);
    }
    if ((g_InputState.pressed & 0x80) || (g_InputState.repeat & 0x80))
    {
        int current = selection->current;
        // Keep VC9's native constant-multiply quotient codegen, not IDIV.
        int remainder = current - (current / 13) * 13;
        MoveSecondaryDebugSelection(selection, remainder == 12 ? -12 : 1);
    }
    if (selection->previous != selection->current) QueueMeshEffectSound(12);
}
__forceinline void SignalChoice(PauseMenuView *self)
{
    SignalSecondaryAnimationEvent(self->root, static_cast<short>(self->selection.current + 7));
}
__forceinline void SignalOpening(PauseMenuView *self)
{
    AnimationVmView *vm = FindAnimationVmById(g_RenderBatch, self->root);
    if (vm)
    {
        reinterpret_cast<SecondaryActorAnimationView *>(vm)->event = 2;
        BoundVmView *bound = reinterpret_cast<BoundVmView *>(vm);
        if (!bound->marker018)
            for (BoundVmNodeView *node = bound->related; node; node = node->next)
                reinterpret_cast<SecondaryActorAnimationView *>(node->vm)->event = 2;
    }
}
__forceinline int CurrentScore()
{
    return *reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(&g_SecondaryHostSelectionState) + 4);
}
__forceinline int &RuntimeMode()
{
    return *reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(&g_MainRuntime) + 0x558);
}
}

void __stdcall UpdatePauseGameOverMenu(PauseContinueMenuView *owner)
{
    PauseMenuView *self = &owner->pause;
    char filename[64];
    switch (self->state)
    {
    case 20:
        if (self->time.current >= 10 && (g_InputState.pressed & 0x80001))
        {
            QueueMeshEffectSound(10);
            self->state = 21;
            SignalOpening(self);
            SetStageTimer(&self->time, 0);
        }
        break;
    case 21:
        if (self->time.current < 10) break;
        CommitPauseMenuScore();
        if (g_StageResourceFlags & 0x10)
        {
            unsigned int character = static_cast<unsigned int>(g_MeshEffectSelection0) * 3u +
                g_MeshEffectSelection1;
            unsigned int stage = static_cast<unsigned int>(g_SecondaryActorDifficulty) * 6u +
                g_StageVariant;
            int *record = reinterpret_cast<int *>(g_SecondaryGameplayStatistics +
                character * 0x68d4u + stage * 8u + 0x5a4);
            if (*record < CurrentScore()) *record = CurrentScore();
            // The game-over/practice display has a different script for
            // each save-mode, unlike the normal continue menu.
            self->root = CreateDefaultSecondaryDrawAnimationVmId(owner->file,
                0x68 - (self->saveMode != 0));
            self->state = 22;
            // Keep the native practice and regular selection resets distinct.
            self->selection.count = 3;
            self->selection.wrapEnabled = 1;
            Choose(0, &self->selection);
        }
        else
        {
            __asm
            {
                mov esi, ebx
                call PreparePauseContinueRecords
            }
            SetStageTimer(&self->time, 0);
            if (!self->unknown1f8)
            {
                self->state = 25;
                HideMenuAnimationTree(self->root);
                return;
            }
            self->root = CreateDefaultSecondaryDrawAnimationVmId(owner->file, 0x62);
            // This path rejoins the ordinary continuation state handler.
            self->state = 14;
            self->selection.count = 3;
            self->selection.wrapEnabled = 1;
            Choose(0, &self->selection);
        }
        ClearSecondaryProjectileHit(&self->root.value);
        SignalChoice(self);
        break;
    case 22:
        Vertical(&self->selection);
        if (self->selection.previous != self->selection.current)
        {
            SignalChoice(self);
            QueueMeshEffectSound(12);
        }
        if (g_InputState.pressed & 0x80001)
        {
            SignalPauseMenuChild(FindSecondaryDialogueChild(&self->root.value,
                self->selection.current + 0x5e));
            QueueMeshEffectSound(10);
            self->state = 26;
            SetStageTimer(&self->time, 0);
            CommitPauseMenuScore();
            switch (self->selection.current)
            {
            case 0:
            case 2:
                self->state = 26;
                SetStageTimer(&self->time, 0);
                QueueMeshEffectSound(10);
                break;
            case 1:
                self->state = 23;
                SetStageTimer(&self->time, 0);
                HideMenuAnimationTree(self->root);
                PushConfigurationSelection(&self->selection);
                self->selection.count = 25;
                self->selection.wrapEnabled = 1;
                Choose(0, &self->selection);
                for (int i = 0; i < 25; ++i)
                {
                    sprintf(filename, "th11_%.2d.rpy", i + 1);
                    self->replays[i] = ReadMenuReplay(filename);
                }
                break;
            }
        }
        if (g_InputState.pressed & 0x102)
        {
            QueueMeshEffectSound(11);
            if (self->selection.current != 0)
            {
                Choose(0, &self->selection);
                SignalChoice(self);
                return;
            }
        }
        break;
    case 23:
        if (self->time.current < 10) break;
        Vertical(&self->selection);
        if (self->selection.previous != self->selection.current) QueueMeshEffectSound(12);
        if (g_InputState.pressed & 0x80001)
        {
            self->state = 24;
            SetStageTimer(&self->time, 0);
            Choose(0, &self->keyboard);
            self->keyboard.count = 91;
            self->keyboard.wrapEnabled = 1;
            MenuReplayView *replay = reinterpret_cast<MenuReplayView *>(g_SecondaryControllerMode);
            ReplaySaveMetadata **header = reinterpret_cast<ReplaySaveMetadata **>(&replay->header);
            if (!self->saveMode || (g_StageResourceFlags & 0x10))
            {
                _time64(reinterpret_cast<__time64_t *>(reinterpret_cast<unsigned char *>(*header) + 0xc));
                (*header)->stage = g_StageVariant;
            }
            else
            {
                _time64(reinterpret_cast<__time64_t *>(reinterpret_cast<unsigned char *>(*header) + 0xc));
                (*header)->stage = 8;
            }
            strcpy(self->name, reinterpret_cast<char *>(g_SecondaryGameplayStatistics) + 0x2dde0);
            self->namePosition = 0;
            if (strcmp(self->name, "        ")) MoveSecondaryDebugSelection(&self->keyboard, -1);
            int length = 8;
            while (length > 0 && self->name[length - 1] == ' ') --length;
            self->namePosition = length;
            QueueMeshEffectSound(10);
            return;
        }
        if (g_InputState.pressed & 0x102)
        {
            self->state = 22;
            SetStageTimer(&self->time, 0);
            ShowPauseMenuAnimationTree(self->root);
            PopConfigurationSelection(&self->selection);
            self->selection.count = 3;
            self->selection.wrapEnabled = 1;
            SignalChoice(self);
            for (int i = 0; i < 25; ++i)
            {
                MenuReplayView *replay = self->replays[i];
                if (replay)
                {
                    DestroyMenuReplayContents(replay);
                    ::operator delete(replay);
                }
                self->replays[i] = 0;
            }
            self->state = 22;
            SetStageTimer(&self->time, 0);
            QueueMeshEffectSound(10);
        }
        break;
    case 24:
        {
            if (self->time.current < 10) break;
            SecondaryDebugSelectionView *keyboard = &self->keyboard;
            Keyboard(keyboard);
            if (g_InputState.pressed & 0x80001)
            {
                int key = keyboard->current;
                if (key < 88)
                {
                    if (self->namePosition < 8)
                    {
                        self->name[self->namePosition] = NameCharacters[key];
                        ++self->namePosition;
                        if (self->namePosition >= 8) SelectMenuItem(90, keyboard);
                    }
                    else self->name[self->namePosition - 1] = NameCharacters[key];
                }
                else if (key == 88)
                {
                    if (self->namePosition < 8)
                    {
                        self->name[self->namePosition] = ' ';
                        ++self->namePosition;
                        if (self->namePosition >= 8) SelectMenuItem(90, keyboard);
                    }
                    else self->name[self->namePosition - 1] = ' ';
                }
                else if (key == 89)
                {
                    if (self->namePosition)
                    {
                        --self->namePosition;
                        self->name[self->namePosition] = ' ';
                        QueueMeshEffectSound(11);
                    }
                    return;
                }
                else if (key == 90)
                {
                    QueueMeshEffectSound(44);
                    sprintf(filename, "th11_%.2d.rpy", self->selection.current + 1);
                    DeletePauseMenuReplay(self->replays[self->selection.current]);
                    WritePauseMenuReplay(filename, self->name, 0);
                    int slot = self->selection.current;
                    self->replays[slot] = ReadMenuReplay(filename);
                    self->state = 23;
                    SetStageTimer(&self->time, 0);
                    strcpy(reinterpret_cast<char *>(g_SecondaryGameplayStatistics) + 0x2dde0, self->name);
                    return;
                }
                QueueMeshEffectSound(10);
            }
            if (g_InputState.pressed & 0x102)
            {
                QueueMeshEffectSound(11);
                if (!self->namePosition)
                {
                    self->state = 23;
                    SetStageTimer(&self->time, 0);
                    return;
                }
                --self->namePosition;
                self->name[self->namePosition] = ' ';
            }
            break;
        }
    case 25:
        {
            if (self->time.current < 10) break;
            if (!self->unknown1f8) Keyboard(&self->keyboard);
            if (g_InputState.pressed & 0x80001)
            {
                if (self->unknown1f8) goto FinishRecordName;
                int key = self->keyboard.current;
                if (key < 88)
                {
                    if (self->namePosition < 8)
                    {
                        self->name[self->namePosition] = NameCharacters[key];
                        ++self->namePosition;
                        if (self->namePosition >= 8) SelectMenuItem(90, &self->keyboard);
                    }
                    else self->name[self->namePosition - 1] = NameCharacters[key];
                }
                else if (key == 88)
                {
                    if (self->namePosition < 8)
                    {
                        self->name[self->namePosition] = ' ';
                        ++self->namePosition;
                        if (self->namePosition >= 8) SelectMenuItem(90, &self->keyboard);
                    }
                    else self->name[self->namePosition - 1] = ' ';
                }
                else if (key == 89)
                {
                    if (!self->namePosition) return;
                    --self->namePosition;
                    self->name[self->namePosition] = ' ';
                }
                else if (key == 90)
                {
                    unsigned int character = static_cast<unsigned int>(g_MeshEffectSelection0) * 3u + g_MeshEffectSelection1;
                    unsigned int row = static_cast<unsigned int>(g_SecondaryActorDifficulty) * 10u + self->selection.current;
                    char *statistics = reinterpret_cast<char *>(g_SecondaryGameplayStatistics);
                    strcpy(statistics + character * 0x68d4u + row * 0x1cu + 0x1e, self->name);
                    strcpy(statistics + 0x2dde0, self->name);
                    goto FinishRecordName;
                }
                QueueMeshEffectSound(10);
                return;
            }
            if (g_InputState.pressed & 0x102)
            {
                if (self->selection.current < 0) goto FinishRecordName;
                if (self->namePosition)
                {
                    QueueMeshEffectSound(11);
                    --self->namePosition;
                    self->name[self->namePosition] = ' ';
                }
            }
            break;
FinishRecordName:
            self->root = CreateDefaultSecondaryDrawAnimationVmId(owner->file, 0x67);
            ShowPauseMenuAnimationTree(self->root);
            self->state = 22;
            // Keep the native practice and regular selection resets distinct.
            self->selection.count = 3;
            self->selection.wrapEnabled = 1;
            Choose(0, &self->selection);
            ClearSecondaryProjectileHit(&self->root.value);
            SignalChoice(self);
            return;
        }
    case 26:
        if (self->time.current < 12) break;
        ClosePauseContinueMenu(self);
        self->state = 27;
        switch (self->selection.current)
        {
        case 0:
        case 1:
            SetSecondaryGameplayRuntimeMode(&g_MainRuntime, 4);
            break;
        case 2:
            RuntimeMode() = 10;
            return;
        }
        break;
    }
}
