#include "../src/SoundQueue.hpp"
extern int __stdcall LoadPreparedMusic(SoundManagerQueueView *manager, int index, const char *filename);
extern "C" int __stdcall PreparedMusicLoadProbe(SoundManagerQueueView *manager,
    int index, const char *filename)
{
    return LoadPreparedMusic(manager, index, filename);
}
