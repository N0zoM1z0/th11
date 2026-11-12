#pragma once
#include "ReplayLeaderboardInitialization.hpp"
#include <stddef.h>

struct ReplayLeaderboardRuntimeView
{
    unsigned char *sourceFile;
    unsigned char *decodedFile;
    ReplayScoreArchiveView scores[7];
    ReplaySeedArchiveView seed;

    ReplayLeaderboardRuntimeView();
};

typedef char ReplayLeaderboardTotalSize[
    (sizeof(ReplayLeaderboardRuntimeView) == 0x2e21c) ? 1 : -1];
typedef char ReplayLeaderboardScoresOffset[
    (offsetof(ReplayLeaderboardRuntimeView, scores) == 8) ? 1 : -1];
typedef char ReplayLeaderboardSeedOffset[
    (offsetof(ReplayLeaderboardRuntimeView, seed) == 0x2ddd4) ? 1 : -1];

extern ReplayLeaderboardRuntimeView *g_ReplayLeaderboardRuntime;
extern const char g_ReplayLeaderboardArchiveName[];
int __stdcall LoadReplayLeaderboardRecords(ReplayLeaderboardRuntimeView *self);
void __cdecl CreateReplayLeaderboardRuntime();
