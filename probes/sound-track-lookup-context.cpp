#include "../src/SoundQueue.hpp"

extern int __stdcall FindSoundTrack(const char *, SoundManagerQueueView *);
extern "C" int __stdcall SoundTrackLookupProbe(const char *filename, SoundManagerQueueView *manager)
{
    return FindSoundTrack(filename, manager);
}
