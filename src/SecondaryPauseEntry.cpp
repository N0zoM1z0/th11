#include "SecondaryPlayerController.hpp"
#include "PauseContinueMenu.hpp"
#include "PauseOverlayRectangle.hpp"
#include "SecondaryDamage.hpp"
#include "SecondaryDialogue.hpp"
#include "SecondaryActorRuntime.hpp"
#include "SecondaryGameplay.hpp"
#include "MusicWaveRequest.hpp"
#include "SoundQueue.hpp"
#include "MainRuntime.hpp"

// These two untyped global anchors refer to the observed native pause-session
// pointer and game-over state root. Their nominal original class owners are
// still unknown; no ABI is inferred from the names.
extern PauseContinueMenuView *g_SecondaryPauseSession;
extern unsigned char *g_SecondaryGameOverStateRoot;

namespace
{
__forceinline AnimationFileView *PauseBackdropFile()
{
    // Game runtime +0x588 is the common pause-backdrop ANM file pointer.
    return *reinterpret_cast<AnimationFileView **>(
        reinterpret_cast<unsigned char *>(&g_MainRuntime) + 0x588);
}

__forceinline AnimationFileView *PauseMainFile()
{
    return *reinterpret_cast<AnimationFileView **>(
        reinterpret_cast<unsigned char *>(g_SecondaryActorHud) + 0x444c);
}

__forceinline void ResetPauseEntryTimers(PauseMenuView *pause)
{
    pause->time.SetFrame(-1);
    pause->secondaryTime.SetFrame(-1);
}

__forceinline void DisablePauseSceneUpdate()
{
    *reinterpret_cast<unsigned int *>(
        reinterpret_cast<unsigned char *>(g_SecondaryDialogueSession) + 0x60) |= 0x10;
}

__forceinline void SavePauseAudioAndTime(PauseMenuView *pause, int audioState)
{
    pause->savedTimeScale = g_AnimationTimeScale;
    g_AnimationTimeScale = 1.0f;
    pause->savedAudioState = g_SecondaryGameplayAudioState;
    g_SecondaryGameplayAudioState = audioState;
}
}

// Native 0x42C760..0x42C872: enter the in-game continue/pause overlay.
void __stdcall EnterSecondaryContinueMenu()
{
    PauseContinueMenuView *session = g_SecondaryPauseSession;
    PauseMenuView *pause = &session->pause;
    pause->state = 2;
    ResetPauseEntryTimers(pause);
    DisablePauseSceneUpdate();

    pause->backdrop = CreateDefaultSecondaryDrawAnimationVmId(PauseBackdropFile(), 0x4b);
    CapturePauseOverlayFromVm(pause->backdrop);
    session->file = PauseMainFile();
    pause->root = CreateDefaultSecondaryDrawAnimationVmId(session->file, 0x5b);
    ClearSecondaryProjectileHit(&pause->root.value);

    if (g_StageVariant != 6 ||
        *reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(g_SecondaryDialogueSession) + 0x14) >= 300)
        EnqueueSoundCommand(&g_SoundManagerQueue, 6, 0, "Pause");

    SavePauseAudioAndTime(pause, 0);
}

// Native 0x42D560..0x42D6AF: game-over entry, with a separate early-exit
// mode-1 path that changes the runtime mode without opening the menu.
void __stdcall EnterSecondaryGameOverMenu()
{
    if (g_SecondaryDialogueSession->mode == 1)
    {
        *reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(&g_MainRuntime) + 0x558) =
            (g_MainRuntime.flags & 0x2000) ? 2 : 4;
        return;
    }

    PauseContinueMenuView *session = g_SecondaryPauseSession;
    PauseMenuView *pause = &session->pause;
    pause->state = (g_StageResourceFlags & 0x10) ? 21 : 13;
    ResetPauseEntryTimers(pause);
    DisablePauseSceneUpdate();

    pause->backdrop = CreateDefaultSecondaryDrawAnimationVmId(PauseBackdropFile(), 0x4b);
    CapturePauseOverlayFromVm(pause->backdrop);
    session->file = PauseMainFile();
    QueueMusicWaveRequest(0, "bgm/th10_17.wav");
    if (reinterpret_cast<unsigned char *>(&g_MainRuntime)[0x200] & 0x10)
        EnqueueSoundCommand(&g_SoundManagerQueue, 4, 0, "dummy");
    EnqueueSoundCommand(&g_SoundManagerQueue, 2, 0, "dummy");

    g_SecondaryGameOverStateRoot[0x2de0b] = 1;
    pause->saveMode = 0;
    SavePauseAudioAndTime(pause, 1);
}

// Native 0x42C620: state-1 pause overlay with mode-selected root script.
void __stdcall EnterSecondaryPauseMenu()
{
    PauseContinueMenuView *session = g_SecondaryPauseSession;
    PauseMenuView *pause = &session->pause;
    pause->state = 1;
    ResetPauseEntryTimers(pause);
    DisablePauseSceneUpdate();

    pause->backdrop = CreateDefaultSecondaryDrawAnimationVmId(PauseBackdropFile(), 0x4b);
    CapturePauseOverlayFromVm(pause->backdrop);
    session->file = PauseMainFile();
    pause->root = CreateDefaultSecondaryDrawAnimationVmId(
        session->file, g_SecondaryDialogueSession->mode == 0 ? 0x59 : 0x5a);
    ClearSecondaryProjectileHit(&pause->root.value);
    QueueMeshEffectSound(0x20);

    if (g_StageVariant != 6 ||
        *reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(g_SecondaryDialogueSession) + 0x14) >= 300)
        EnqueueSoundCommand(&g_SoundManagerQueue, 6, 0, "Pause");

    SavePauseAudioAndTime(pause, 0);
}

// Native 0x42D6B0: state-21 game-over overlay and practice music setup.
void __stdcall EnterSecondaryGameOverPracticeMenu()
{
    if (g_SecondaryDialogueSession->mode == 1)
    {
        *reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(&g_MainRuntime) + 0x558) =
            (g_MainRuntime.flags & 0x2000) ? 2 : 4;
        return;
    }

    PauseContinueMenuView *session = g_SecondaryPauseSession;
    PauseMenuView *pause = &session->pause;
    pause->state = 21;
    ResetPauseEntryTimers(pause);
    DisablePauseSceneUpdate();

    pause->backdrop = CreateDefaultSecondaryDrawAnimationVmId(PauseBackdropFile(), 0x4b);
    CapturePauseOverlayFromVm(pause->backdrop);
    session->file = PauseMainFile();
    QueueMusicWaveRequest(0, "bgm/th10_17.wav");
    if (reinterpret_cast<unsigned char *>(&g_MainRuntime)[0x200] & 0x10)
        EnqueueSoundCommand(&g_SoundManagerQueue, 4, 0, "dummy");
    EnqueueSoundCommand(&g_SoundManagerQueue, 2, 0, "dummy");

    g_SecondaryGameOverStateRoot[0x2de0b] = 1;
    pause->saveMode = 0;
    SavePauseAudioAndTime(pause, 1);
}
