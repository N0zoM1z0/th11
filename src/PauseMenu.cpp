#include "PauseMenu.hpp"
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

struct ReplaySaveMetadata
{
    unsigned char unknown000[0x68];
    int stage;
};
typedef char ReplaySaveStageOffset[(offsetof(ReplaySaveMetadata, stage) == 0x68) ? 1 : -1];

__forceinline void Choose(int value, SecondaryDebugSelectionView *selection)
{
    if (selection->count)
        selection->current = value >= selection->count ?
            static_cast<int>(static_cast<unsigned int>(selection->count) - 1) :
            value < 0 ? 0 : value;
    else selection->current = value;
}

struct SelectionExclusions
{
    unsigned char unknown000[0x90];
    int excluded[16];
    int wrapEnabled, excludedCount;
};
__forceinline void Exclude(SecondaryDebugSelectionView *selection, int value)
{
    SelectionExclusions *storage = reinterpret_cast<SelectionExclusions *>(selection);
    storage->excluded[storage->excludedCount++] = value;
}
__forceinline void Vertical(SecondaryDebugSelectionView *selection)
{
    selection->previous = selection->current;
    if ((g_InputState.pressed & 0x10) || (g_InputState.repeat & 0x10))
        MoveSecondaryDebugSelection(selection, -1);
    if ((g_InputState.pressed & 0x20) || (g_InputState.repeat & 0x20))
        MoveSecondaryDebugSelection(selection, 1);
}
__forceinline void SignalChoice(PauseMenuView *self, int base)
{
    SignalSecondaryAnimationEvent(self->root, static_cast<short>(self->selection.current + base));
}
__forceinline void SignalOpening(PauseMenuView *self)
{
    AnimationVmView *vm = FindAnimationVmById(g_RenderBatch, self->root);
    if (vm)
    {
        reinterpret_cast<SecondaryActorAnimationView *>(vm)->event = 14;
        BoundVmView *bound = reinterpret_cast<BoundVmView *>(vm);
        if (!bound->marker018)
            for (BoundVmNodeView *node = bound->related; node; node = node->next)
                reinterpret_cast<SecondaryActorAnimationView *>(node->vm)->event = 14;
    }
}
__forceinline void CancelToResume(PauseMenuView *self)
{
    Choose(0, &self->selection);
    SignalSecondaryAnimation(&self->backdrop.value);
    SignalSecondaryAnimation(&self->root.value);
    self->state = 4;
    SetStageTimer(&self->time, 0);
}
__forceinline int &RuntimeMode()
{
    return *reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(&g_MainRuntime) + 0x558);
}
}

void __stdcall UpdatePauseMenu(PauseMenuView *self)
{
    char filename[64];
    switch (self->state)
    {
    case 1:
        if (self->time.current < 10) break;
        self->state = 3;
        self->selection.count = 4;
        if (g_SecondaryDialogueSession->mode) Exclude(&self->selection, 2);
        self->selection.wrapEnabled = 1;
        Choose(0, &self->selection);
        SignalChoice(self, 7);
        self->openingKind = 0;
        break;
    case 2:
        if (self->time.current < 10) break;
        self->state = 3;
        self->selection.count = 4;
        Exclude(&self->selection, 2);
        Exclude(&self->selection, 0);
        self->selection.wrapEnabled = 1;
        Choose(1, &self->selection);
        SignalChoice(self, 7);
        self->openingKind = 1;
        break;
    case 3:
        Vertical(&self->selection);
        if (self->selection.previous != self->selection.current)
        {
            SignalChoice(self, 7);
            QueueMeshEffectSound(12);
        }
        if (g_InputState.pressed & 0x80001)
        {
            QueueMeshEffectSound(10);
            switch (self->selection.current)
            {
            case 0:
                SignalSecondaryAnimation(&self->backdrop.value);
                SignalSecondaryAnimation(&self->root.value);
                self->state = 4;
                break;
            case 1:
                SignalPauseMenuChild(FindSecondaryDialogueChild(&self->root.value, 0x4e));
                self->state = 5 - (g_SecondaryDialogueSession->mode != 0);
                break;
            case 2:
                SignalPauseMenuChild(FindSecondaryDialogueChild(&self->root.value, 0x4f));
                self->state = 7;
                break;
            case 3:
                SignalPauseMenuChild(FindSecondaryDialogueChild(&self->root.value, 0x50));
                self->state = 5;
                self->state = 5 - (g_SecondaryDialogueSession->mode != 0);
                break;
            }
            SetStageTimer(&self->time, 0);
        }
        if (!self->openingKind && (g_InputState.pressed & 0x200000))
        {
            QueueMeshEffectSound(10);
            SignalPauseMenuChild(FindSecondaryDialogueChild(&self->root.value, 0x50));
            self->state = 4;
            SetStageTimer(&self->time, 0);
            Choose(3, &self->selection);
            SignalSecondaryAnimation(&self->backdrop.value);
        }
        if (g_InputState.pressed & 0x10000)
        {
            QueueMeshEffectSound(10);
            SignalPauseMenuChild(FindSecondaryDialogueChild(&self->root.value, 0x4e));
            self->state = 4;
            SetStageTimer(&self->time, 0);
            Choose(1, &self->selection);
        }
        if (self->openingKind) break;
        if (g_InputState.pressed & 0x100) CancelToResume(self);
        break;
    case 4:
        if (self->time.current < 12) break;
        self->state = 0;
        switch (self->selection.current)
        {
        case 0: ResumePauseMenu(self); break;
        case 1:
            SignalSecondaryAnimation(&self->backdrop.value);
            SignalSecondaryAnimation(&self->root.value);
            SetSecondaryGameplayRuntimeMode(&g_MainRuntime, 4);
            break;
        case 3:
            SignalSecondaryAnimation(&self->backdrop.value);
            SignalSecondaryAnimation(&self->root.value);
            RuntimeMode() = 10 + (g_SecondaryDialogueSession->mode != 0);
            break;
        }
        break;
    case 5:
    case 7:
        if (self->time.current < 20) break;
        if (self->time.current == 20)
        {
            PushConfigurationSelection(&self->selection);
            self->selection.count = 2;
            self->selection.wrapEnabled = 1;
            Choose(1, &self->selection);
            SignalOpening(self);
        }
        if (self->time.current < 30) break;
        if (self->time.current == 30) SignalChoice(self, 15);
        Vertical(&self->selection);
        if (self->selection.previous != self->selection.current)
        {
            SignalChoice(self, 15);
            QueueMeshEffectSound(12);
        }
        if (g_InputState.pressed & 0x80001)
        {
            QueueMeshEffectSound(10);
            switch (self->selection.current)
            {
            case 0:
                SignalPauseMenuChild(FindSecondaryDialogueChild(&self->root.value, 0x57));
                self->state = (self->state == 7) * 2 + 6;
                break;
            case 1:
                SignalPauseMenuChild(FindSecondaryDialogueChild(&self->root.value, 0x58));
                self->state = 6;
                break;
            }
            SetStageTimer(&self->time, 0);
        }
        if (g_InputState.pressed & 0x100) CancelToResume(self);
        break;
    case 6:
        if (self->time.current < 20) break;
        switch (self->selection.current)
        {
        case 0:
            SignalSecondaryAnimation(&self->root.value);
            self->state = 4;
            PopConfigurationSelection(&self->selection);
            break;
        case 1:
            PopConfigurationSelection(&self->selection);
            SignalChoice(self, 7);
            self->state = 3;
            SetStageTimer(&self->time, 0);
            return;
        }
        SetStageTimer(&self->time, 0);
        break;
    case 8:
        if (self->time.current < 20) break;
        self->state = 9;
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
    case 9:
        if (self->time.current < 10) break;
        Vertical(&self->selection);
        if (self->selection.previous != self->selection.current) QueueMeshEffectSound(12);
        if (g_InputState.pressed & 0x80001)
        {
            self->state = 10;
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
            self->state = 14;
            SetStageTimer(&self->time, 0);
            PopConfigurationSelection(&self->selection);
            self->selection.count = 4;
            self->selection.wrapEnabled = 1;
            SignalChoice(self, 7);
            for (int i = 0; i < 25; ++i)
            {
                DeletePauseMenuReplay(self->replays[i]);
                self->replays[i] = 0;
            }
            self->state = 4;
            Choose(1, &self->selection);
            SetStageTimer(&self->time, 0);
            QueueMeshEffectSound(11);
        }
        break;
    case 10:
        if (self->time.current < 10) break;
        SecondaryDebugSelectionView *keyboard = &self->keyboard;
        keyboard->previous = keyboard->current;
        if ((g_InputState.pressed & 0x10) || (g_InputState.repeat & 0x10))
            MoveSecondaryDebugSelection(keyboard, -13);
        if ((g_InputState.pressed & 0x20) || (g_InputState.repeat & 0x20))
            MoveSecondaryDebugSelection(keyboard, 13);
        if ((g_InputState.pressed & 0x40) || (g_InputState.repeat & 0x40))
            MoveSecondaryDebugSelection(keyboard, keyboard->current % 13 == 0 ? 12 : -1);
        if ((g_InputState.pressed & 0x80) || (g_InputState.repeat & 0x80))
            MoveSecondaryDebugSelection(keyboard, keyboard->current % 13 == 12 ? -12 : 1);
        if (keyboard->previous != keyboard->current) QueueMeshEffectSound(12);
        if (g_InputState.pressed & 0x80001)
        {
            int key = keyboard->current;
            if (key < 88 || key == 88)
            {
                char value = key < 88 ? NameCharacters[key] : ' ';
                if (self->namePosition < 8)
                {
                    self->name[self->namePosition] = value;
                    ++self->namePosition;
                    if (self->namePosition >= 8) SelectMenuItem(90, keyboard);
                }
                else self->name[self->namePosition - 1] = value;
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
                WritePauseMenuReplay(filename, self->name, 1);
                int slot = self->selection.current;
                self->replays[slot] = ReadMenuReplay(filename);
                self->state = 9;
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
                self->state = 9;
                SetStageTimer(&self->time, 0);
                return;
            }
            --self->namePosition;
            self->name[self->namePosition] = ' ';
        }
        break;
    }
}
