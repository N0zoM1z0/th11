#pragma once
#include "SecondaryActorState.hpp"

void __stdcall QuantizeSecondaryMotionPosition(SecondaryMotionView *self);
void __stdcall RotateSecondaryVector(SecondaryVector2 *output, const SecondaryVector2 *input, float angle);
