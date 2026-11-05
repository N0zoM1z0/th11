#pragma once
#include "SecondaryPairAccess.hpp"

// Source building blocks for the complete 412E30 host. These declarations do
// not claim that its instruction switch or original class/TU is reconstructed.
extern CameraVector g_SecondaryHostVectorPreset;
void __stdcall ResetSecondaryHostVectorTimer(StageVectorTween *self);
void __stdcall RecomposeSecondaryHostMotion(SecondaryMotionView *self);
void __stdcall ResetSecondaryHostVector(CameraVector *self);
float __stdcall SecondaryHostRandomAngle();
float __stdcall AnimationExtentMagnitude(float value);
float __stdcall NormalizeAngle(float value);

typedef char SecondaryHostLocalOffset[(offsetof(SecondaryActorStateView, local) == 0x68) ? 1 : -1];
typedef char SecondaryHostOffsetOffset[(offsetof(SecondaryActorStateView, offset) == 0x9c) ? 1 : -1];
typedef char SecondaryHostOffsetTweenOffset[(offsetof(SecondaryActorStateView, offsetTween) == 0x1c0) ? 1 : -1];
typedef char SecondaryHostPairTweensOffset[(offsetof(SecondaryActorStateView, pairTweens) == 0x20c) ? 1 : -1];
typedef char SecondaryHostBoundsOffset[(offsetof(SecondaryActorStateView, boundX) == 0x14dc) ? 1 : -1];
typedef char SecondaryHostPairModeOffset[(offsetof(SecondaryPairTween, mode) == 0x38) ? 1 : -1];
typedef char SecondaryHostVectorModeOffset[(offsetof(StageVectorTween, mode) == 0x48) ? 1 : -1];

namespace SecondaryHostInstructions
{
const double Missing = -999999.0;
const double HalfPi = 1.5707963705062866;
const double Pi = 3.1415927410125732;
const double TwoPi = 6.2831854820251465;

__forceinline float Real(SecondaryActorStateView *self, int index)
{ return ReadSecondaryFloatArgument(self->owner->current, index); }
__forceinline int Integer(SecondaryActorStateView *self, int index)
{ return ReadSecondaryIntArgument(self->owner->current, index); }

// The x87 branches reject unordered inputs as well as the sentinel range.
__forceinline bool Present(float value) { return value > Missing; }
__forceinline float MirroredAngle(float value)
{
    float difference = static_cast<float>(value - HalfPi);
    float wrapped = NormalizeAngle(difference);
    difference = static_cast<float>(HalfPi - wrapped);
    return NormalizeAngle(difference);
}
__forceinline void PairControls(SecondaryPairTween *tween)
{
    tween->control1 = g_SecondaryPairControlPreset;
    tween->control2 = g_SecondaryPairControlPreset;
}
__forceinline void Pair(SecondaryPairTween *tween, const SecondaryVector2 &initial,
    const SecondaryVector2 &target, int duration, int mode)
{
    tween->duration = duration;
    PairControls(tween);
    tween->initial = initial;
    tween->mode = mode;
    tween->target = target;
    ResetSecondaryPairTimer(tween);
}
__forceinline void RefreshWorldPosition(SecondaryActorStateView *self)
{
    self->world.position.x = self->offset.position.x + self->local.position.x;
    self->world.position.y = self->offset.position.y + self->local.position.y;
    self->world.position.z = self->offset.position.z + self->local.position.z;
}
}
