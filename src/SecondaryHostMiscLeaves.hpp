#pragma once
#include "Camera.hpp"

struct SecondaryHostEffectSnapshot;
struct SecondaryHostVectorStorage;
void __cdecl PauseSecondaryMasterChains();
void __stdcall ReleaseSecondaryHostSpellAnimation();
SecondaryHostEffectSnapshot *__fastcall ResetSecondaryHostEffectSnapshot(
    SecondaryHostEffectSnapshot *self);
void __fastcall CopySecondaryHostVector(SecondaryHostVectorStorage *self,
    const CameraVector *position);
