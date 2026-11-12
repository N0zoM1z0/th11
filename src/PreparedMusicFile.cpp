#include "SoundQueue.hpp"
#include "MusicStream.hpp"

extern int __stdcall FindSoundTrack(const char *name, SoundManagerQueueView *self);
extern void __cdecl TraceSoundQueue(const char *format, ...);
extern const char g_PreparedMusicFileTrace[];

namespace
{
__forceinline MusicControllerView *&PreparedController(SoundManagerQueueView *self)
{
    return *reinterpret_cast<MusicControllerView **>(
        reinterpret_cast<unsigned char *>(self) + 0x5208);
}
}

// TH11 0x00449C20..0x00449C60: select a cached track for a live controller.
int __stdcall StartPreparedMusicFile(SoundManagerQueueView *self,
    const char *filename)
{
    if (!PreparedController(self)) return -1;
    int index = FindSoundTrack(filename, self);
    MusicControllerView *controller = PreparedController(self);
    ConfigureMusicDecoder(controller->decoder, self->records + index, 0);
    TraceSoundQueue(g_PreparedMusicFileTrace, index);
    return 0;
}
