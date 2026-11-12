#include "../src/SoundQueue.hpp"
extern int __stdcall StartPreparedMusicFile(SoundManagerQueueView *, const char *);
extern "C" int __stdcall PreparedMusicFileProbe(SoundManagerQueueView *manager,
    const char *filename)
{
    return StartPreparedMusicFile(manager, filename);
}
