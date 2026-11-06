#pragma once

struct SecondaryHudView;

int __stdcall DrawSecondaryHud(SecondaryHudView *self);
int __fastcall SecondaryHudDrawCallback(SecondaryHudView *self);
void __stdcall DecodeSecondaryHudBestTime(int *seconds, int *hundredths);

// Four-float left/top/right/bottom rectangle and packed ARGB color.
void __stdcall DrawHudSolidRectangle(const float *rectangle, unsigned int color);
