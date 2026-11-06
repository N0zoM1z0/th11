#pragma once
#include "SecondaryDialogue.hpp"
#include "Chain.hpp"
#include <string.h>

// Observed fields of the HUD passed to 41B380; original class/size are open.
struct SecondaryHudView
{
    unsigned char unknown000[0x10];
    AnimationVmView primary[9], digits[2], gauges[4], indicator;
    unsigned char unknown4350[0x4380 - 0x4350];
    unsigned int title, badges[10];
    unsigned char unknown43ac[0x43bc - 0x43ac];
    unsigned int tracked;
    int trackedActive, unknown43c4;
    StageTimerView time;
    unsigned char unknown43dc[0x43ec - 0x43dc];
    AnimationFileView *file;
    float bossDisplayed, bossTarget;
    int bossValue, badgeCount;
    SecondaryHostHudGauge bars[4];
    unsigned int flags;
    StageTimerView transition;
    SecondaryDialogueView *dialogue;
    unsigned int unknown443c;
    int seconds, unknown4444, previousSeconds;
    AnimationFileView *portraitFile;
    int clearBonus, clearFrame;
};

// The 0x44-byte screen-effect allocation and its timer are observed at both
// 4489E0 and this caller; field names below are physical observations.
struct SecondaryHudScreenEffect
{
    unsigned int flags;
    unsigned int unknown004;
    ChainElement *update, *draw;
    int mode;
    unsigned int unknown014, value018;
    int duration;
    unsigned int color0, color1, value028, unknown02c;
    StageTimerView time;
    SecondaryHudScreenEffect()
    {
        memset(this, 0, sizeof(*this));
        flags |= 2;
    }
};
void __stdcall InitializeSecondaryHudScreenEffect(SecondaryHudScreenEffect *self,
    int mode, int duration, int color0, int color1, int drawPriority);

SecondaryHudScreenEffect *__stdcall CreateSecondaryHudScreenEffect(
    int mode, int duration, int color0, int color1, int drawPriority);

int __stdcall UpdateSecondaryHud(SecondaryHudView *self);
int __fastcall SecondaryHudUpdateCallback(SecondaryHudView *self);

typedef char SecondaryHudDigitsOffset[(offsetof(SecondaryHudView, digits) == 0x25e4) ? 1 : -1];
typedef char SecondaryHudGaugesOffset[(offsetof(SecondaryHudView, gauges) == 0x2e4c) ? 1 : -1];
typedef char SecondaryHudIndicatorOffset[(offsetof(SecondaryHudView, indicator) == 0x3f1c) ? 1 : -1];
typedef char SecondaryHudTitleOffset[(offsetof(SecondaryHudView, title) == 0x4380) ? 1 : -1];
typedef char SecondaryHudTimeOffset[(offsetof(SecondaryHudView, time) == 0x43c8) ? 1 : -1];
typedef char SecondaryHudBarsOffset[(offsetof(SecondaryHudView, bars) == 0x4400) ? 1 : -1];
typedef char SecondaryHudFlagsOffset[(offsetof(SecondaryHudView, flags) == 0x4420) ? 1 : -1];
typedef char SecondaryHudDialogueOffset[(offsetof(SecondaryHudView, dialogue) == 0x4438) ? 1 : -1];
typedef char SecondaryHudPortraitOffset[(offsetof(SecondaryHudView, portraitFile) == 0x444c) ? 1 : -1];
typedef char SecondaryHudClearFrameOffset[(offsetof(SecondaryHudView, clearFrame) == 0x4454) ? 1 : -1];
typedef char SecondaryHudScreenSize[(sizeof(SecondaryHudScreenEffect) == 0x44) ? 1 : -1];

typedef char SecondaryHudScreenUpdateOffset[(offsetof(SecondaryHudScreenEffect, update) == 8) ? 1 : -1];
typedef char SecondaryHudScreenTimeOffset[(offsetof(SecondaryHudScreenEffect, time) == 0x30) ? 1 : -1];
