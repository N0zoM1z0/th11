#pragma once
#include "SecondaryActorState.hpp"

// Observed state and vector contracts; original class ownership is unresolved.
extern SecondaryVector2 g_SecondaryPairControlPreset;
void __stdcall SetSecondaryPairMode(SecondaryPairTween *self, int mode);
void __stdcall SetSecondaryPairInitial(SecondaryPairTween *self, const SecondaryVector2 *value);
void __stdcall SetSecondaryPairTarget(SecondaryPairTween *self, const SecondaryVector2 *value);
void __stdcall ResetSecondaryPairControl1(SecondaryPairTween *self);
void __stdcall ResetSecondaryPairControl2(SecondaryPairTween *self);
void __stdcall SetSecondaryPairDuration(SecondaryPairTween *self, int duration);
void __stdcall ResetSecondaryPairTimer(SecondaryPairTween *self);
int __stdcall GetSecondaryPairDuration(const SecondaryPairTween *self);
void __stdcall StopSecondaryPairTween(SecondaryPairTween *self);
SecondaryVector2 *__stdcall SubtractSecondaryPair(SecondaryVector2 *output,
    const SecondaryVector2 *left, const SecondaryVector2 *right);
SecondaryVector2 *__stdcall ScaleSecondaryPair(SecondaryVector2 *output,
    const SecondaryVector2 *input, float scale);
SecondaryVector2 *__stdcall AddSecondaryPair(SecondaryVector2 *output,
    const SecondaryVector2 *left, const SecondaryVector2 *right);
