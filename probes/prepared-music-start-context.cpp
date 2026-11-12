#include "../src/SoundQueue.hpp"
extern int __stdcall StartPreparedMusic(SoundManagerQueueView *, int);
extern "C" int __stdcall PreparedMusicStartProbe(SoundManagerQueueView *manager, int index)
{
    return StartPreparedMusic(manager, index);
}
