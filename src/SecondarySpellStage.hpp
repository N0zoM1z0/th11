#pragma once

#include "StageAuxiliary.hpp"
#include "AnimationVm.hpp"
#include <stddef.h>

// Physical 0x91C-byte prefix of the stage spell presenter at the
// existing 0x4A8D6C singleton. Its original C++ class name is unknown.
struct SecondarySpellStageView
{
    unsigned char unknown000[0x10];
    AnimationVmView vm0;
    AnimationVmView vm1;
    unsigned int followerIds[3];
    unsigned int positionVmId;
    StageTimerView time;
    char name[0x40];
    int spellIndex;
    unsigned int flags;
    int score, scoreInitial, scoreDuration, unknown8f0;
    int activeFrames;
    unsigned char unknown8f8[0x910 - 0x8f8];
    CameraVector position;
};

typedef char SpellStageSecondVmOffset[(offsetof(SecondarySpellStageView, vm1) == 0x444) ? 1 : -1];
typedef char SpellStageFollowersOffset[(offsetof(SecondarySpellStageView, followerIds) == 0x878) ? 1 : -1];
typedef char SpellStageTimerOffset[(offsetof(SecondarySpellStageView, time) == 0x888) ? 1 : -1];
typedef char SpellStageNameOffset[(offsetof(SecondarySpellStageView, name) == 0x89c) ? 1 : -1];
typedef char SpellStageIndexOffset[(offsetof(SecondarySpellStageView, spellIndex) == 0x8dc) ? 1 : -1];
typedef char SpellStageFlagsOffset[(offsetof(SecondarySpellStageView, flags) == 0x8e0) ? 1 : -1];
typedef char SpellStagePositionOffset[(offsetof(SecondarySpellStageView, position) == 0x910) ? 1 : -1];

void __stdcall InitializeSecondarySpellStage(const char *spellName, int spellIndex, int duration);
int __stdcall UpdateSecondarySpellStageState(SecondarySpellStageView *self);
