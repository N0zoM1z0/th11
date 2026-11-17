#pragma once

#include "SecondarySpellStage.hpp"
#include "ChainRegistration.hpp"

// Owning C++ class behind the existing physical SecondarySpellStageView.
// It has actual construction/destruction of the two 0x434-byte animation VMs.
// The physical view used by drawing/update remains a separate, non-owning
// projection of this same object; no second singleton is allocated.
struct SecondarySpellStageOwned
{
    unsigned int flags;
    unsigned int unknown004;
    ChainElement *updateChain;
    ChainElement *drawChain;
    StageEmbeddedVm animationVms[2];
    unsigned int followerIds[3];
    unsigned int positionVmId;
    StageTimerView time;
    unsigned char remaining89c[0x924 - 0x89c];

    __forceinline SecondarySpellStageOwned();
    ~SecondarySpellStageOwned();
};

typedef char SecondarySpellOwnedSize[
    (sizeof(SecondarySpellStageOwned) == 0x924) ? 1 : -1];
typedef char SecondarySpellOwnedVmOffset[
    (offsetof(SecondarySpellStageOwned, animationVms) == 0x10) ? 1 : -1];
typedef char SecondarySpellOwnedDrawChainOffset[
    (offsetof(SecondarySpellStageOwned, drawChain) == 0x0c) ? 1 : -1];
typedef char SecondarySpellOwnedIdsOffset[
    (offsetof(SecondarySpellStageOwned, followerIds) == 0x878) ? 1 : -1];
typedef char SecondarySpellOwnedTimerOffset[
    (offsetof(SecondarySpellStageOwned, time) == 0x888) ? 1 : -1];

int __stdcall RegisterSecondarySpellStageChains(SecondarySpellStageOwned *self);
SecondarySpellStageOwned *CreateSecondarySpellStage();
SecondarySpellStageOwned *DeleteSecondarySpellStageReturningPointer(
    SecondarySpellStageOwned *self);
void DestroySecondarySpellStage();
int __fastcall SecondarySpellUpdateCallback(void *self);
int __fastcall SecondarySpellDrawCallback(void *self);
