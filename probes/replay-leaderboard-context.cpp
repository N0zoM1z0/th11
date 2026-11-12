#include "ReplayLeaderboardInitialization.hpp"
// Independent dynamic receivers keep the complete archive initializers
// visible to VC9 without simulating private register state.
extern "C" void __stdcall ReplayLeaderboardProbe(
    ReplayScoreArchiveView *scores, ReplaySeedArchiveView *seeds, int which)
{
    if (which) InitializeReplayScoreArchive(scores);
    else InitializeReplaySeedArchive(seeds);
}
