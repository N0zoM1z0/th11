#include "../src/SecondaryReplayState.hpp"
extern int __stdcall LoadReplayStageData(ReplayInitStateView *, void *);
extern "C" int __stdcall SecondaryReplayLoadProbe(ReplayInitStateView *state, void *source)
{
    return LoadReplayStageData(state, source);
}
