#pragma once
#include <stddef.h>

// The replay polling path passes a different physical input block from the
// 0x459B50 UI repeater. Its counters start at +0x94, and key masks at +0x118.
struct ReplayInputRepeatView
{
    unsigned char unknown000[0x94];
    unsigned int heldFrames[32];
    unsigned int unknown114;
    unsigned int current, previous, repeat, pressed, released, unknown12c, held;
};
typedef char ReplayInputRepeatSize[(sizeof(ReplayInputRepeatView) == 0x134) ? 1 : -1];
typedef char ReplayInputRepeatCountersOffset[(offsetof(ReplayInputRepeatView, heldFrames) == 0x94) ? 1 : -1];
typedef char ReplayInputRepeatCurrentOffset[(offsetof(ReplayInputRepeatView, current) == 0x118) ? 1 : -1];
typedef char ReplayInputRepeatHeldOffset[(offsetof(ReplayInputRepeatView, held) == 0x130) ? 1 : -1];
void __fastcall UpdateReplayInputRepeatState(ReplayInputRepeatView *self);
