#include "SoundQueue.hpp"
#include "MainRuntime.hpp"

// game.cpp music pause/resume helpers called by UpdateSecondaryGameplay. The
// names follow their old declarations in SecondaryGameplay.hpp: stage variant
// 6 starts its music and pauses it at once (0x420BA0), then resumes it at frame
// 300 (0x420B80).
void __cdecl StartSecondaryGameplayExtraAudio()
{
    EnqueueSoundCommand(&g_SoundManagerQueue, 6, 0, "Pause");
}

void __cdecl AdvanceSecondaryGameplayExtraAudio()
{
    EnqueueSoundCommand(&g_SoundManagerQueue, 7, 0, "UnPause");
}

// Byte +0x200 of the main runtime holds option flags; bit 4 selects command 4
// before the stage music starts and when it is reset.
namespace
{
__forceinline unsigned char RuntimeOptionFlags()
{
    return *(reinterpret_cast<const unsigned char *>(&g_MainRuntime) + 0x200);
}
}

extern unsigned char *g_SecondaryGameplayStatistics;

// 0x42A240
int __cdecl ResetSecondaryGameplayAudio()
{
    if (RuntimeOptionFlags() & 0x10)
        EnqueueSoundCommand(&g_SoundManagerQueue, 4, 0, "dummy");
    else
        EnqueueSoundCommand(&g_SoundManagerQueue, 3, 0, "dummy");
    return 0;
}

// 0x42A150: start track `value` and mark track `selection` as heard in the
// saved statistics.
int __stdcall StartSecondaryGameplayAudio(int value, int selection)
{
    if (RuntimeOptionFlags() & 0x10)
        EnqueueSoundCommand(&g_SoundManagerQueue, 4, 0, "dummy");
    EnqueueSoundCommand(&g_SoundManagerQueue, 2, value, "dummy");
    g_SecondaryGameplayStatistics[selection + 0x2ddfa] = 1;
    return 0;
}
