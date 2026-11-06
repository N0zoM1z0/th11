#pragma once
#include "SecondaryActorInstructionControl.hpp"
#include "SecondaryPlayerWrapItems.hpp"
#include "FrameLoop.hpp"
#include "InputBackend.hpp"
#include "AsciiLifecycle.hpp"

// Observed fields only; the original gameplay/session classes remain unknown.
struct SecondaryGameplayView
{
    unsigned char unknown000[0x10];
    StageTimerView time;
    unsigned char unknown024[0x60 - 0x24];
    unsigned int flags;
};
struct SecondaryGameplayHudView
{
    unsigned char unknown000[0x4370];
    unsigned int signal4370;
    unsigned char unknown4374[0x4420 - 0x4374];
    unsigned int flags;
    int unknown4424, transitionFrame;
    unsigned char unknown442c[0x4438 - 0x442c];
    void *dialogue;
};
struct SecondaryGameplayChainsView
{
    unsigned char unknown000[8];
    ChainElement *update, *draw;
};
struct SecondaryGameplaySelectionView
{
    unsigned char unknown000[0x34];
    int value;
};
struct SecondaryGameplaySettingsView
{
    unsigned char unknown000[0x14];
    int value014, value018;
    StageTimerView time;
};
extern SecondaryGameplaySettingsView g_SecondaryGameplaySettings;
extern SecondaryGameplaySelectionView *g_SecondaryGameplaySelectionEC8;
extern SecondaryGameplayChainsView *g_SecondaryGameplayChainsD74, *g_SecondaryGameplayChainsEC0;
extern unsigned char *g_SecondaryGameplayStatistics;
extern unsigned int g_SecondaryGameplayFrame0, g_SecondaryGameplayFrame1;
extern int g_SecondaryGameplayAudioState;

int __stdcall UpdateSecondaryGameplay(SecondaryGameplayView *self);
int __fastcall SecondaryGameplayUpdateCallback(SecondaryGameplayView *self);

// Reviewed call boundaries, with conventional source ABIs. Original nominal
// types and the target's LTCG register allocation are separate questions.
void __cdecl ResetSecondaryGameplayHud();
void __cdecl ResetSecondaryGameplayStage();
void __cdecl StartSecondaryGameplayStageTransition();
void __cdecl ResetSecondaryGameplayStageTimer();
void __cdecl ResetSecondaryGameplayProjectiles();
void __cdecl ResetSecondaryGameplayPlayer();
void __stdcall ResetSecondaryGameplayActors(SecondaryMasterView *self);
void __cdecl ReleaseSecondaryGameplayObjects();
void __cdecl ResetSecondaryGameplayMode();
int __cdecl InitializeSecondaryGameplayHud();
void __cdecl PauseSecondaryGameplayBoss();
void __cdecl PauseSecondaryGameplayPlayer();
void __stdcall PauseSecondaryGameplayObject(void *self);
void __cdecl PauseSecondaryGameplayActors();
void __cdecl PauseSecondaryGameplayItems();
void __cdecl PauseSecondaryGameplaySceneEffect();
int __stdcall StartSecondaryGameplayAudio(int value, int selection);
int __cdecl ResetSecondaryGameplayAudio();
void __cdecl StartSecondaryGameplayExtraAudio();
void __cdecl AdvanceSecondaryGameplayExtraAudio();
void __cdecl ClearSecondaryGameplayAsciiOverlay();
void __stdcall SetSecondaryGameplayRuntimeMode(MainRuntimeView *runtime, int mode);
void __cdecl UpdateSecondaryGameplayHud();
void __stdcall UpdateSecondaryGameplayGauges(SecondaryGameplaySettingsView *settings);

typedef char SecondaryGameplayGaugeValueOffset[(offsetof(SecondaryGameplaySettingsView, value014) == 0x14) ? 1 : -1];
typedef char SecondaryGameplayGaugeTimerOffset[(offsetof(SecondaryGameplaySettingsView, time) == 0x1c) ? 1 : -1];
typedef char SecondaryGameplayTimeOffset[(offsetof(SecondaryGameplayView, time) == 0x10) ? 1 : -1];
typedef char SecondaryGameplayFlagsOffset[(offsetof(SecondaryGameplayView, flags) == 0x60) ? 1 : -1];
typedef char SecondaryGameplayHudFlagsOffset[(offsetof(SecondaryGameplayHudView, flags) == 0x4420) ? 1 : -1];
typedef char SecondaryGameplayHudFrameOffset[(offsetof(SecondaryGameplayHudView, transitionFrame) == 0x4428) ? 1 : -1];
typedef char SecondaryGameplayHudDialogueOffset[(offsetof(SecondaryGameplayHudView, dialogue) == 0x4438) ? 1 : -1];

struct SecondaryDebugSelectionView
{
    int current, previous, count;
    unsigned char unknown00c[0xd0 - 0xc];
    int wrapEnabled, excludedCount;
};
struct SecondaryEclDebugView
{
    unsigned char unknown000[0x10];
    RuntimeThreadView worker;
    unsigned int unknown02c;
    int state;
    char **filenames;
    int fileCount;
    SecondaryDebugSelectionView selections[3];
    unsigned char unknown2c4[0x6fc - 0x2c4];
    float value6fc, value700, value704;
    int value708;
    unsigned int flags;
};
// The reviewed entry has no explicit argument reads, EAX0 and plain RET.
// Its nominal callback type and forwarded-callee inputs remain open.
typedef unsigned int (__cdecl *SecondaryDebugThreadCallback)();
int __fastcall UpdateSecondaryEclDebug(SecondaryEclDebugView *self);
void __cdecl ResetSecondaryDebugSession();
int __stdcall MoveSecondaryDebugSelection(SecondaryDebugSelectionView *selection, int amount);
void __stdcall StartSecondaryDebugThread(RuntimeThreadView *thread,
    SecondaryDebugThreadCallback callback, void *context);
unsigned int __cdecl LoadSecondaryDebugScript();
void __cdecl DeleteSecondaryDebugHud();
void __cdecl DeleteSecondaryDebugPlayer();
void __cdecl DeleteSecondaryDebugProjectiles();
void __cdecl DeleteSecondaryDebugEnemy();
void __cdecl DeleteSecondaryDebugItems();
void __cdecl DeleteSecondaryDebugChainsEC0();
void __cdecl DeleteSecondaryMaster();
void __cdecl ResetSecondaryGameplayItems();
void __stdcall UpdateSecondaryDebugInput(InputStateView *input);

typedef char SecondaryDebugSelectionSize[(sizeof(SecondaryDebugSelectionView) == 0xd8) ? 1 : -1];
typedef char SecondaryDebugWorkerOffset[(offsetof(SecondaryEclDebugView, worker) == 0x10) ? 1 : -1];
typedef char SecondaryDebugStateOffset[(offsetof(SecondaryEclDebugView, state) == 0x30) ? 1 : -1];
typedef char SecondaryDebugSelectionOffset[(offsetof(SecondaryEclDebugView, selections) == 0x3c) ? 1 : -1];
typedef char SecondaryDebugValuesOffset[(offsetof(SecondaryEclDebugView, value6fc) == 0x6fc) ? 1 : -1];
typedef char SecondaryDebugFlagsOffset[(offsetof(SecondaryEclDebugView, flags) == 0x70c) ? 1 : -1];
