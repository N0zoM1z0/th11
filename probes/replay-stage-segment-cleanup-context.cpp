struct ReplayCleanupState;
extern void __stdcall ClearReplayStageSegments(ReplayCleanupState *, int);
extern "C" void __stdcall ReplayStageSegmentCleanupProbe(ReplayCleanupState *state, int stage)
{
    ClearReplayStageSegments(state, stage);
}
