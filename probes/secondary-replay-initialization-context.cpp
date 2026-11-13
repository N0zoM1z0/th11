#include "../src/SecondaryReplayState.hpp"
extern int __stdcall InitializeSecondaryReplay(ReplayInitStateView *state,
    int mode, void *source);
extern "C" int __stdcall SecondaryReplayInitializationProbe(ReplayInitStateView *state,
    int mode, void *source)
{
    return InitializeSecondaryReplay(state, mode, source);
}
