#pragma once

#include <windows.h>
#include "SoundBuffer.hpp"
#include <stddef.h>

struct SoundCommand
{
    int opcode, value, step;
    char text[256];
};
struct SoundEffectQueue
{
    int ids[12], counts[12], pans[12][128];
};
struct SoundTrackRecord
{
    unsigned char unknown000[0x1c];
    int loop;
    unsigned char unknown020[0x34 - 0x20];
};
struct MusicDecoderView
{
    unsigned char unknown000[0x90];
    SoundTrackRecord *record;
};
// Partial polymorphic prefix, observed scalar-deleting destructor in vtable slot0.
struct MusicControllerView
{
    virtual ~MusicControllerView();
    SoundBufferView **buffers;
    unsigned int bufferBytes;
    MusicDecoderView *decoder;
    unsigned int bufferCount;
    int fadeRemaining, fadeInitial, fadeEnabled;
    unsigned int playPriority, playFlags, unknown028, unknown02c;
    int active;
    unsigned char unknown034[0x78 - 0x34];
    volatile int busy;
};
struct SoundManagerQueueView
{
    unsigned char unknown000[0x610];
    int enabled;
    DWORD threadId;
    HANDLE thread;
    unsigned int unknown61c;
    SoundEffectQueue effects;
    unsigned char unknown1e80[0x1f84 - 0x1e80];
    SoundTrackRecord *records;
    SoundCommand commands[32];
    char filenames[16][256];
};
struct SoundVolumeRecord { short value; unsigned char unknown002[6]; };

typedef char SoundCommandSize[(sizeof(SoundCommand) == 0x10c) ? 1 : -1];
typedef char SoundQueueOffset[(offsetof(SoundManagerQueueView, effects) == 0x620) ? 1 : -1];
typedef char SoundCommandOffset[(offsetof(SoundManagerQueueView, commands) == 0x1f88) ? 1 : -1];
typedef char SoundFilenameOffset[(offsetof(SoundManagerQueueView, filenames) == 0x4108) ? 1 : -1];
typedef char SoundRecordSize[(sizeof(SoundTrackRecord) == 0x34) ? 1 : -1];
typedef char MusicDecoderOffset[(offsetof(MusicControllerView, decoder) == 0xc) ? 1 : -1];
typedef char MusicBusyOffset[(offsetof(MusicControllerView, busy) == 0x78) ? 1 : -1];

extern SoundManagerQueueView g_SoundManagerQueue;
extern MusicControllerView *g_MusicController;
extern HANDLE g_MusicEvent;
extern int g_MusicVolume, g_SoundEffectVolume;
extern SoundBufferView *g_SoundEffectBuffers[];
extern const SoundVolumeRecord g_SoundVolumeRecords[];
extern const char g_SoundFadeTrace[];

int __cdecl ProcessSoundQueue();
DWORD WINAPI MusicStreamingThread(void *context);
void __stdcall EnqueueSoundCommand(SoundManagerQueueView *self, int opcode,
    int value, const char *text);
SoundBufferView *__stdcall FirstMusicBuffer(MusicControllerView *self);
void __stdcall SetMusicBufferVolume(MusicControllerView *self, int baseVolume);
