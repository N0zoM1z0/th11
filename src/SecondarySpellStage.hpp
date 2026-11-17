#pragma once

#include "StageAuxiliary.hpp"
#include "AnimationVm.hpp"
#include <stddef.h>

// Physical 0x91C-byte prefix of the stage spell presenter at the
// existing 0x4A8D6C singleton. Its original C++ class name is unknown.
// Native x86 stores the two double fields at +8FC and +904; force the
// observed four-byte packing for this physical view, not the host ABI.
#pragma pack(push, 4)
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
    int score, scoreInitial, scoreDuration, recordIndex;
    int activeFrames, frameCountAtFinish;
    double clockStarted, roundedElapsed;
    int recordCode;
    CameraVector position;
};
#pragma pack(pop)

typedef char SecondarySpellStageSize[(sizeof(SecondarySpellStageView) == 0x91c) ? 1 : -1];
typedef char SpellStageSecondVmOffset[(offsetof(SecondarySpellStageView, vm1) == 0x444) ? 1 : -1];
typedef char SpellStageFollowersOffset[(offsetof(SecondarySpellStageView, followerIds) == 0x878) ? 1 : -1];
typedef char SpellStageTimerOffset[(offsetof(SecondarySpellStageView, time) == 0x888) ? 1 : -1];
typedef char SpellStageNameOffset[(offsetof(SecondarySpellStageView, name) == 0x89c) ? 1 : -1];
typedef char SpellStageIndexOffset[(offsetof(SecondarySpellStageView, spellIndex) == 0x8dc) ? 1 : -1];
typedef char SpellStageFlagsOffset[(offsetof(SecondarySpellStageView, flags) == 0x8e0) ? 1 : -1];
typedef char SpellStageClockStartOffset[(offsetof(SecondarySpellStageView, clockStarted) == 0x8fc) ? 1 : -1];
typedef char SpellStageRecordCodeOffset[(offsetof(SecondarySpellStageView, recordCode) == 0x90c) ? 1 : -1];
typedef char SpellStagePositionOffset[(offsetof(SecondarySpellStageView, position) == 0x910) ? 1 : -1];

void __stdcall InitializeSecondarySpellStage(const char *spellName, int spellIndex, int duration);
int __stdcall UpdateSecondarySpellStageState(SecondarySpellStageView *self);
int __stdcall DrawSecondarySpellStage(SecondarySpellStageView *self);

int __stdcall CheckSecondarySpellRecordCode(const SecondarySpellStageView *self);
void __stdcall SettleSecondarySpellStageTime();
