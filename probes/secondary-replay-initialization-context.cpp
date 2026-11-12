#include "../src/SecondaryPattern.hpp"
struct ReplayInitStateView;
extern int __stdcall InitializeSecondaryReplay(ReplayInitStateView *, int, void *);
extern "C" int __stdcall SecondaryReplayInitializationProbe(ReplayInitStateView *state,
    int mode, void *source)
{
    return InitializeSecondaryReplay(state, mode, source);
}
