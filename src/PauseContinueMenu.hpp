#pragma once
#include "PauseMenu.hpp"
#include "SecondaryEmitterAnimation.hpp"

// Observed continuation of the pause storage; this is not an original class claim.
struct PauseContinueMenuView
{
    PauseMenuView pause;
    AnimationFileView *file;
};
typedef char PauseContinueFileOffset[(offsetof(PauseContinueMenuView, file) == 0x2dc) ? 1 : -1];

void __stdcall UpdatePauseContinueMenu(PauseContinueMenuView *self);
void __cdecl CommitPauseMenuScore();
void __stdcall ClosePauseContinueMenu(PauseMenuView *self);
void __stdcall ShowPauseMenuAnimationTree(const AnimationVmId &id);
AnimationVmId __stdcall CreateDefaultSecondaryDrawAnimationVmId(AnimationFileView *file, int script);

// Reviewed full380-byte dependency at42D850. Its source remains open.
void __stdcall PreparePauseContinueRecords(PauseMenuView *self);
