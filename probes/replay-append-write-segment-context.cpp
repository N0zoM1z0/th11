#include "../src/SecondaryReplayState.hpp"

extern unsigned int g_StageVariant;

// Non-runnable link context: append for the current stage, as the replay
// stage owners do, and keep working after the call.
extern "C" int __stdcall ReplayAppendWriteSegmentProbe(
    ReplayStateView *state, int stage)
{
    if (stage)
        AppendReplayWriteSegment(state, g_StageVariant);
    AppendReplayWriteSegment(g_SecondaryReplayCurrent, g_StageVariant);
    return state->segmentCount;
}
