#include "MusicWaveRequest.hpp"
#include "SoundQueue.hpp"
#include <string.h>

// Native 0x0042A0D0..0x0042A14A. As in the original, this requires the
// filename to contain '.', and does not guard or truncate the local copy.
int __stdcall QueueMusicWaveRequest(int selection, const char *filename)
{
    char path[256];
    strcpy(path, filename);
    char *extension = strrchr(path, '.');
    extension[1] = 'w';
    extension[2] = 'a';
    extension[3] = 'v';
    EnqueueSoundCommand(&g_SoundManagerQueue, 1, selection, path);
    return 1;
}
