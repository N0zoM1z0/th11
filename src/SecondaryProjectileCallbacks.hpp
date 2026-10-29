#pragma once
#include "SecondaryPattern.hpp"

int __fastcall InitializeSecondaryProjectileTarget(SecondaryDamageReferencesView *self,
    SecondaryProjectileView *projectile, unsigned int context);
int __fastcall InitializeSecondaryProjectileAngle(SecondaryDamageReferencesView *self,
    SecondaryProjectileView *projectile, unsigned int context);
int __fastcall InitializeSecondaryProjectileAcceleration(SecondaryDamageReferencesView *self,
    SecondaryProjectileView *projectile, unsigned int context);
int __fastcall UpdateSecondaryProjectileGravity(SecondaryDamageReferencesView *self,
    SecondaryProjectileView *projectile);
int __fastcall PositionSecondaryProjectileFromEmitter(SecondaryDamageReferencesView *self,
    SecondaryProjectileView *projectile);
// Only EDX is observed as an input; its original nominal callback ABI is open.
int __stdcall UpdateSecondaryProjectileHomingLong(SecondaryProjectileView *projectile);
void __stdcall DetachSecondaryProjectileTargets(SecondaryActorManagerView *target);
int __stdcall SpawnSecondaryFixedProjectile(const CameraVector *position, float angle);
int __stdcall SecondaryPatternTimerChanged(const StageTimerView *time);
int __cdecl GetSecondaryPatternMultiplier();
void __stdcall SetSecondaryPatternMultiplier(int value);
void __stdcall RewindSecondaryTimer(StageTimerView *time, int frames);

extern SecondaryProjectileParameters g_SecondaryFixedProjectile;
