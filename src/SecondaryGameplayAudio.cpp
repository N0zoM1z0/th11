#include "SoundQueue.hpp"

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
