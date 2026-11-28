#pragma once
#include "PauseMenu.hpp"
#include "SecondaryEmitterAnimation.hpp"

// Observed continuation of the pause storage; this is not an original class claim.
struct PauseContinueMenuView
{
    PauseContinueMenuView();
    ~PauseContinueMenuView();
    int Update();

    PauseMenuView pause;
    AnimationFileView *file;
};
typedef char PauseContinueFileOffset[(offsetof(PauseContinueMenuView, file) == 0x2dc) ? 1 : -1];

extern PauseContinueMenuView *g_SecondaryPauseSession;

void __stdcall UpdatePauseContinueMenu(PauseContinueMenuView *self);
// Full game-over state20..26 owner, separate from continuation states12..19.
void __stdcall UpdatePauseGameOverMenu(PauseContinueMenuView *self);
void __cdecl CommitPauseMenuScore();
void __stdcall ClosePauseContinueMenu(PauseMenuView *self);
void __stdcall ShowPauseMenuAnimationTree(const AnimationVmId &id);
AnimationVmId __stdcall CreateDefaultSecondaryDrawAnimationVmId(AnimationFileView *file, int script);

// 0042D850; LTCG passes the menu in ESI.
void __stdcall PreparePauseContinueRecords(PauseMenuView *self);
