#include "ReplayLeaderboardRuntime.hpp"
// A genuine dynamic constructor/reader caller for scoped /GL diagnostics.
extern "C" int __stdcall ReplayLeaderboardRuntimeProbe(ReplayLeaderboardRuntimeView *self)
{
    CreateReplayLeaderboardRuntime();
    return LoadReplayLeaderboardRecords(self);
}
