#include "SoundQueue.hpp"
#include <string.h>

// 004496B0..00449786: obtain the leaf name of an audio request, then
// search zero-terminated 0x34-byte soundtrack records. No bound on the
// caller string or number of records is imposed by the native function.
int __stdcall FindSoundTrack(const char *filename, SoundManagerQueueView *self)
{
    int index = 0;
    char localName[128];
    const char *leaf = strrchr(filename, '/');
    if (!leaf) leaf = strrchr(filename, '\\');
    if (!leaf)
        strcpy(localName, filename);
    else
        strcpy(localName, leaf + 1);

    if (self->records[0].name[0])
    {
        do
        {
            if (strcmp(self->records[index].name, localName) == 0)
                break;
            ++index;
        } while (self->records[index].name[0]);
    }
    if (!self->records[index].name[0]) index = 0;
    return index;
}
