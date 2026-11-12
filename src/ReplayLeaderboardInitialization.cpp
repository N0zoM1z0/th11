#include "ReplayLeaderboardInitialization.hpp"
#include "MainRuntime.hpp"
#include "Random.hpp"
#include <string.h>

extern signed char g_ReplayScoreCharacterClass[180];
extern const char g_ReplayScoreEmptyName[9];
extern const char g_ReplaySeedEmptyName[9];
extern RandomStateView g_TextRandomState;

namespace
{
template <typename T> __forceinline T &At(void *base, unsigned int offset)
{
    return *reinterpret_cast<T *>(reinterpret_cast<unsigned char *>(base) + offset);
}

// The original inline generator takes and releases the exact runtime random
// critical section around every sampled word, even for a single table fill.
__forceinline unsigned short NextSeedWord()
{
    if (g_MainRuntime.flags & 0x8000)
    {
        EnterCriticalSection(&g_MainRuntime.criticalSections[10]);
        ++g_MainRuntime.criticalDepth[10];
    }
    ++g_TextRandomState.calls;
    unsigned short value = (g_TextRandomState.seed ^ 0x9630) - 0x6553;
    g_TextRandomState.seed = (value >> 14) + (value << 2);
    if (g_MainRuntime.flags & 0x8000)
    {
        LeaveCriticalSection(&g_MainRuntime.criticalSections[10]);
        --g_MainRuntime.criticalDepth[10];
    }
    return g_TextRandomState.seed;
}
}

// Full native 0x437550..0x437657: CR score archive initialization, 5x10
// descending-score cells and a separate 35x5-character index name table.
void __fastcall InitializeReplayScoreArchive(ReplayScoreArchiveView *self)
{
    memset(self, 0, sizeof(*self));
    At<unsigned short>(self, 0) = 0x5243;
    At<unsigned short>(self, 2) = 0;
    At<unsigned int>(self, 8) = sizeof(*self);

    unsigned char *entry = self->bytes + 0x14;
    int group = 5;
    for (; group != 0; --group)
    {
        int score = 1000000;
        do
        {
            *reinterpret_cast<int *>(entry - 4) = score;
            entry[0] = 1;
            *reinterpret_cast<unsigned int *>(entry + 2) =
                *reinterpret_cast<const unsigned int *>(g_ReplayScoreEmptyName);
            *reinterpret_cast<unsigned int *>(entry + 6) =
                *reinterpret_cast<const unsigned int *>(g_ReplayScoreEmptyName + 4);
            entry[10] = g_ReplayScoreEmptyName[8];
            *reinterpret_cast<int *>(entry + 0x0c) = 0;
            *reinterpret_cast<int *>(entry + 0x10) = 0;
            entry[1] = 0;
            score -= 100000;
            entry += 0x1c;
        } while (score > 0);
    }

    unsigned char *record = self->bytes + 0x6f0;
    for (; group < 175; group += 5, record += 0x2d0)
    {
        int idx = group;
        *reinterpret_cast<int *>(record - 4) = idx;
        *reinterpret_cast<int *>(record) = g_ReplayScoreCharacterClass[idx];
        *reinterpret_cast<int *>(record + 0x8c) = idx + 1;
        *reinterpret_cast<int *>(record + 0x90) = g_ReplayScoreCharacterClass[idx + 1];
        *reinterpret_cast<int *>(record + 0x11c) = idx + 2;
        *reinterpret_cast<int *>(record + 0x120) = g_ReplayScoreCharacterClass[idx + 2];
        *reinterpret_cast<int *>(record + 0x1ac) = idx + 3;
        *reinterpret_cast<int *>(record + 0x1b0) = g_ReplayScoreCharacterClass[idx + 3];
        *reinterpret_cast<int *>(record + 0x23c) = idx + 4;
        *reinterpret_cast<int *>(record + 0x240) = g_ReplayScoreCharacterClass[idx + 4];
    }
}

// Full native 0x437660..0x437736: ST archive with 512 random sample words
// written after a 0x46-byte header. No random seed reset occurs here.
void __fastcall InitializeReplaySeedArchive(ReplaySeedArchiveView *self)
{
    memset(self, 0, sizeof(*self));
    At<unsigned short>(self, 0) = 0x5453;
    At<unsigned short>(self, 2) = 0;
    At<unsigned int>(self, 8) = sizeof(*self);
    memcpy(&At<char>(self, 0xc), g_ReplaySeedEmptyName, 9);
    unsigned short *cursor = &At<unsigned short>(self, 0x46);
    int remaining = 512;
    do
    {
        *cursor = NextSeedWord();
        ++cursor;
    } while (--remaining != 0);
}
