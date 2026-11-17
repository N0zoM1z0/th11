#pragma once

#include "StageResource.hpp"
#include "Chain.hpp"
#include <stddef.h>

struct AnimationFileView;

// Owning C++ view of the already-existing HUD singleton at 0x4A8D84.
// Sixteen VM objects are genuine C++ members: 9+2+4 array VMs
// followed by one independently constructed persistent VM.
struct SecondaryDebugHudOwned
{
    unsigned int flags;
    unsigned int unknown004;
    ChainElement *updateChain;
    ChainElement *drawChain;
    StageEmbeddedVm mainVms[9];
    StageEmbeddedVm auxiliaryVms[2];
    StageEmbeddedVm overlayVms[4];
    StageEmbeddedVm persistentVm; // The 16th native VM at +0x3F1C.
    unsigned int trackedVmIds[8];
    unsigned int precleanupAnimationIds[2];
    unsigned char unknown4378[0x4380 - 0x4378];
    unsigned int precleanupAdditionalIds[11];
    unsigned int effectVmId;
    unsigned char unknown43b0[0x43b8 - 0x43b0];
    unsigned int transitionVmId;
    unsigned char unknown43bc[0x43c8 - 0x43bc];
    StageTimerView hudTimer;
    ChainElement *overlayChain;
    unsigned char unknown43e0[0x43e4 - 0x43e0];
    int displayedValue43e4;
    unsigned char unknown43e8[0x43ec - 0x43e8];
    AnimationFileView *retainedAnimationFile;
    unsigned char unknown43f0[0x43fc - 0x43f0];
    unsigned int reset43fc;
    unsigned char unknown4400[0x4420 - 0x4400];
    unsigned int hudStateFlags;
    StageTimerView transitionTimer;
    void *dialogue;
    void *preparedData;
    int value4440, unknown4444, value4448;
    int ownerMarker;
    unsigned char unknown4450[8];

    SecondaryDebugHudOwned();
    ~SecondaryDebugHudOwned();
};

typedef char SecondaryDebugHudArray0Offset[
    (offsetof(SecondaryDebugHudOwned, mainVms) == 0x10) ? 1 : -1];
typedef char SecondaryDebugHudArray1Offset[
    (offsetof(SecondaryDebugHudOwned, auxiliaryVms) == 0x25e4) ? 1 : -1];
typedef char SecondaryDebugHudArray2Offset[
    (offsetof(SecondaryDebugHudOwned, overlayVms) == 0x2e4c) ? 1 : -1];
typedef char SecondaryDebugHudPersistentVmOffset[
    (offsetof(SecondaryDebugHudOwned, persistentVm) == 0x3f1c) ? 1 : -1];
typedef char SecondaryDebugHudBufferOffset[
    (offsetof(SecondaryDebugHudOwned, persistentVm) + offsetof(StageEmbeddedVm, storage400) == 0x431c) ? 1 : -1];
typedef char SecondaryDebugHudIdsOffset[
    (offsetof(SecondaryDebugHudOwned, trackedVmIds) == 0x4350) ? 1 : -1];
typedef char SecondaryDebugHudOverlayOffset[
    (offsetof(SecondaryDebugHudOwned, overlayChain) == 0x43dc) ? 1 : -1];
typedef char SecondaryDebugHudExtraIdsOffset[
    (offsetof(SecondaryDebugHudOwned, precleanupAdditionalIds) == 0x4380) ? 1 : -1];
typedef char SecondaryDebugHudRetainedFileOffset[
    (offsetof(SecondaryDebugHudOwned, retainedAnimationFile) == 0x43ec) ? 1 : -1];
typedef char SecondaryDebugHudStateOffset[
    (offsetof(SecondaryDebugHudOwned, hudStateFlags) == 0x4420) ? 1 : -1];
typedef char SecondaryDebugHudDialogueOffset[
    (offsetof(SecondaryDebugHudOwned, dialogue) == 0x4438) ? 1 : -1];
typedef char SecondaryDebugHudPreparedOffset[
    (offsetof(SecondaryDebugHudOwned, preparedData) == 0x443c) ? 1 : -1];
typedef char SecondaryDebugHudTimerOffset[
    (offsetof(SecondaryDebugHudOwned, hudTimer) == 0x43c8) ? 1 : -1];
typedef char SecondaryDebugHudTransitionTimerOffset[
    (offsetof(SecondaryDebugHudOwned, transitionTimer) == 0x4424) ? 1 : -1];
typedef char SecondaryDebugHudFlag43d8Offset[
    (offsetof(SecondaryDebugHudOwned, hudTimer) + offsetof(StageTimerView, flags) == 0x43d8) ? 1 : -1];
typedef char SecondaryDebugHudFlag4434Offset[
    (offsetof(SecondaryDebugHudOwned, transitionTimer) + offsetof(StageTimerView, flags) == 0x4434) ? 1 : -1];
typedef char SecondaryDebugHudDisplayedValueOffset[
    (offsetof(SecondaryDebugHudOwned, displayedValue43e4) == 0x43e4) ? 1 : -1];
typedef char SecondaryDebugHudValue4440Offset[
    (offsetof(SecondaryDebugHudOwned, value4440) == 0x4440) ? 1 : -1];
typedef char SecondaryDebugHudValue4448Offset[
    (offsetof(SecondaryDebugHudOwned, value4448) == 0x4448) ? 1 : -1];
typedef char SecondaryDebugHudMarkerOffset[
    (offsetof(SecondaryDebugHudOwned, ownerMarker) == 0x444c) ? 1 : -1];
typedef char SecondaryDebugHudSize[
    (sizeof(SecondaryDebugHudOwned) == 0x4458) ? 1 : -1];

void __stdcall PrepareSecondaryDebugHudDestruction(SecondaryDebugHudOwned *self);
int __stdcall RegisterSecondaryHudChains(SecondaryDebugHudOwned *self);
SecondaryDebugHudOwned *CreateSecondaryDebugHud();

void ClearSecondaryDebugHudResources();
void DestroySecondaryDebugHudSingleton();
