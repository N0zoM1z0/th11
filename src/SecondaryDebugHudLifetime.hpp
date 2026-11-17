#pragma once

#include "StageResource.hpp"
#include "Chain.hpp"
#include <stddef.h>

struct AnimationFileView;

// Owning C++ view of the already-existing HUD singleton at 0x4A8D84.
// All 15 animation VMs are ordinary StageEmbeddedVm members so VC9
// itself constructs/destroys the native 0x434-byte arrays.
struct SecondaryDebugHudOwned
{
    unsigned int flags;
    unsigned int unknown004;
    ChainElement *updateChain;
    ChainElement *drawChain;
    StageEmbeddedVm mainVms[9];
    StageEmbeddedVm auxiliaryVms[2];
    StageEmbeddedVm overlayVms[4];
    unsigned char unknown3f1c[0x431c - 0x3f1c];
    void *ownedData;
    unsigned char unknown4320[0x4350 - 0x4320];
    unsigned int trackedVmIds[8];
    unsigned int precleanupAnimationIds[2];
    unsigned char unknown4378[0x4380 - 0x4378];
    unsigned int precleanupAdditionalIds[11];
    unsigned int effectVmId;
    unsigned char unknown43b0[0x43b8 - 0x43b0];
    unsigned int transitionVmId;
    unsigned char unknown43bc[0x43dc - 0x43bc];
    ChainElement *overlayChain;
    unsigned char unknown43e0[0x43ec - 0x43e0];
    AnimationFileView *retainedAnimationFile;
    unsigned char unknown43f0[0x43fc - 0x43f0];
    unsigned int reset43fc;
    unsigned char unknown4400[0x4420 - 0x4400];
    unsigned int hudStateFlags;
    unsigned char unknown4424[0x4438 - 0x4424];
    void *dialogue;
    void *preparedData;
    unsigned char unknown4440[0x444c - 0x4440];
    int ownerMarker;

    ~SecondaryDebugHudOwned();
};

typedef char SecondaryDebugHudArray0Offset[
    (offsetof(SecondaryDebugHudOwned, mainVms) == 0x10) ? 1 : -1];
typedef char SecondaryDebugHudArray1Offset[
    (offsetof(SecondaryDebugHudOwned, auxiliaryVms) == 0x25e4) ? 1 : -1];
typedef char SecondaryDebugHudArray2Offset[
    (offsetof(SecondaryDebugHudOwned, overlayVms) == 0x2e4c) ? 1 : -1];
typedef char SecondaryDebugHudBufferOffset[
    (offsetof(SecondaryDebugHudOwned, ownedData) == 0x431c) ? 1 : -1];
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
typedef char SecondaryDebugHudMarkerOffset[
    (offsetof(SecondaryDebugHudOwned, ownerMarker) == 0x444c) ? 1 : -1];
typedef char SecondaryDebugHudSize[
    (sizeof(SecondaryDebugHudOwned) == 0x4450) ? 1 : -1];

void __stdcall PrepareSecondaryDebugHudDestruction(SecondaryDebugHudOwned *self);
