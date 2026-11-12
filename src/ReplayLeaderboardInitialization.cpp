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

    for (int category = 0; category < 5; ++category)
    {
        for (int rank = 0; rank < 10; ++rank)
        {
            unsigned int offset = 0x10 + (category * 10 + rank) * 0x1c;
            At<unsigned int>(self, offset) = 1000000 - rank * 100000;
            At<unsigned char>(self, offset + 4) = 1;
            At<unsigned char>(self, offset + 5) = 0;
            memcpy(&At<char>(self, offset + 6), g_ReplayScoreEmptyName, 9);
            At<unsigned int>(self, offset + 0x10) = 0;
            At<unsigned int>(self, offset + 0x14) = 0;
        }
    }

    for (int group = 0; group < 35; ++group)
    {
        unsigned int base = 0x6ec + group * 0x2d0;
        for (int index = 0; index < 5; ++index)
        {
            int id = group * 5 + index;
            At<int>(self, base + index * 0x90) = id;
            At<int>(self, base + index * 0x90 + 4) = g_ReplayScoreCharacterClass[id];
        }
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
