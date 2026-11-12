#include "../src/SoundQueue.hpp"
extern void __stdcall ClosePreparedMusic(SoundManagerQueueView *);
extern "C" void __stdcall PreparedMusicCloseProbe(SoundManagerQueueView *manager)
{
    ClosePreparedMusic(manager);
}
