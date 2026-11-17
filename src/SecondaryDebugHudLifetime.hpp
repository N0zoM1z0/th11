#pragma once

#include "StageResource.hpp"
#include "Chain.hpp"
#include <stddef.h>

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
    unsigned char unknown4370[0x43ac - 0x4370];
    unsigned int effectVmId;
    unsigned char unknown43b0[0x43b8 - 0x43b0];
    unsigned int transitionVmId;
    unsigned char unknown43bc[0x43dc - 0x43bc];
    ChainElement *overlayChain;
    unsigned char unknown43e0[0x444c - 0x43e0];
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
typedef char SecondaryDebugHudMarkerOffset[
    (offsetof(SecondaryDebugHudOwned, ownerMarker) == 0x444c) ? 1 : -1];
typedef char SecondaryDebugHudSize[
    (sizeof(SecondaryDebugHudOwned) == 0x4450) ? 1 : -1];
