#pragma once
#include "SecondaryPlayerController.hpp"

struct SecondaryLivesDisplayView
{
    unsigned char unknown000[0x10];
    AnimationVmView lives[9];
};

extern int g_SecondaryScoreFraction, g_SecondaryScoreFloor, g_SecondaryLifeFragments;
void __stdcall UpdateSecondaryLivesDisplay(SecondaryActorHudView *hud, int lives, int fragments);
void __stdcall ApplySecondaryPlayerDeathPenalty(const CameraVector *position);

typedef char SecondaryLivesDisplayOffset[(offsetof(SecondaryLivesDisplayView, lives) == 0x10) ? 1 : -1];
