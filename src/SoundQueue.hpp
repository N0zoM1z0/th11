#pragma once

#include <windows.h>
#include "SoundDevice.hpp"
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
    char name[16];
    DWORD dataOffset, storedBytes;
    int loopStart;
    DWORD dataSize;
    WORD formatTag, channels;
    DWORD samplesPerSecond, bytesPerSecond;
    WORD blockAlign, bitsPerSample, extraBytes, unknown032;
};
struct MusicDecoderView
{
    unsigned char unknown000[8];
    DWORD remaining;
    unsigned char unknown00c[0x2c - 0xc];
    DWORD dataSize;
    unsigned char unknown030[0x78 - 0x30];
    int closeOnStop, memoryMode;
    const unsigned char *memoryStart, *cursor;
    DWORD memorySize;
    HANDLE file;
    SoundTrackRecord *record;
    const char *filename;
    DWORD savedFilePosition;
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
    int paused;
    SoundBufferDescription bufferDescription;
    SoundDeviceOwnerView *deviceOwner;
    DWORD lastPlayPosition, playProgress, nextWriteOffset;
    int fillSilence;
    DWORD notificationBytes;
    HANDLE notificationEvent;
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
typedef char MusicRecordDataSizeOffset[(offsetof(SoundTrackRecord, dataSize) == 0x1c) ? 1 : -1];
typedef char MusicRecordBitsOffset[(offsetof(SoundTrackRecord, bitsPerSample) == 0x2e) ? 1 : -1];
typedef char MusicDecoderRecordOffset[(offsetof(MusicDecoderView, record) == 0x90) ? 1 : -1];
typedef char MusicNotificationOffset[(offsetof(MusicControllerView, notificationBytes) == 0x70) ? 1 : -1];
typedef char MusicDescriptionOffset[(offsetof(MusicControllerView, bufferDescription) == 0x38) ? 1 : -1];
typedef char MusicDeviceOwnerOffset[(offsetof(MusicControllerView, deviceOwner) == 0x5c) ? 1 : -1];
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
