#pragma once
#include "MenuController.hpp"
#include "SecondaryDialogue.hpp"
#include "TitleMenuRecord.hpp"
#include "ReplayAllocator.hpp"

// Observed pause-menu storage; neither the original class nor its full size is known.
struct PauseMenuView
{
    int unknown000, state;
    unsigned char unknown008[8];
    StageTimerView time, secondaryTime;
    SecondaryDebugSelectionView selection, keyboard;
    AnimationVmId root, backdrop;
    int namePosition, saveMode, unknown1f8, openingKind, savedAudioState;
    TitleMenuRecordView *replays[25];
    unsigned char unknown268[0x2cc - 0x268];
    char name[9];
    unsigned char unknown2d5[3];
    float savedTimeScale;
};

typedef char PauseMenuStateOffset[(offsetof(PauseMenuView, state) == 4) ? 1 : -1];
typedef char PauseMenuTimeOffset[(offsetof(PauseMenuView, time) == 0x10) ? 1 : -1];
typedef char PauseMenuSelectionOffset[(offsetof(PauseMenuView, selection) == 0x38) ? 1 : -1];
typedef char PauseMenuKeyboardOffset[(offsetof(PauseMenuView, keyboard) == 0x110) ? 1 : -1];
typedef char PauseMenuRootOffset[(offsetof(PauseMenuView, root) == 0x1e8) ? 1 : -1];
typedef char PauseMenuOpeningKindOffset[(offsetof(PauseMenuView, openingKind) == 0x1fc) ? 1 : -1];
typedef char PauseMenuReplaysOffset[(offsetof(PauseMenuView, replays) == 0x204) ? 1 : -1];
typedef char PauseMenuNameOffset[(offsetof(PauseMenuView, name) == 0x2cc) ? 1 : -1];
typedef char PauseMenuSavedScaleOffset[(offsetof(PauseMenuView, savedTimeScale) == 0x2d8) ? 1 : -1];

void __stdcall UpdatePauseMenu(PauseMenuView *self);
void __stdcall ResumePauseMenu(PauseMenuView *self);
void __stdcall SignalPauseMenuChild(const AnimationVmId &id);
// Native 435FC0 receives its record in ESI and has no stack arguments.
void __stdcall DeletePauseMenuReplay();
__forceinline void DeletePauseMenuReplayRecord(TitleMenuRecordView *record)
{
    __asm
    {
        mov esi, record
        call DeletePauseMenuReplay
    }
}

// Reviewed source-open replay writer at 436420; return value is not consumed here.
void __fastcall WritePauseMenuReplay(const char *filename, const char *name, int mode);
