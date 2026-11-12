#include "SoundQueue.hpp"
#include "MainRuntime.hpp"
#include "RuntimeCritical.hpp"
#include <stdlib.h>
#include <string.h>

// These format strings have independently observed .rdata references at
// 0x497140 and 0x49715C; keep their data ownership external.
extern const char g_PreparedMusicPreloadTrace[];
extern const char g_PreparedMusicMissingFileTrace[];
void __cdecl TraceSoundQueue(const char *format, ...);
int __stdcall FindSoundTrack(const char *name, SoundManagerQueueView *self);

namespace
{
__forceinline unsigned char *&LoadedBuffer(SoundManagerQueueView *self, int index)
{
    return reinterpret_cast<unsigned char **>(
        reinterpret_cast<unsigned char *>(self) + 0x1ec0)[index];
}
__forceinline unsigned char *&PlaybackBuffer(SoundManagerQueueView *self, int index)
{
    return reinterpret_cast<unsigned char **>(
        reinterpret_cast<unsigned char *>(self) + 0x1f00)[index];
}
__forceinline SoundTrackRecord *&CachedTrack(SoundManagerQueueView *self, int index)
{
    return reinterpret_cast<SoundTrackRecord **>(
        reinterpret_cast<unsigned char *>(self) + 0x1e80)[index];
}
__forceinline DWORD &CachedBytes(SoundManagerQueueView *self, int index)
{
    return reinterpret_cast<DWORD *>(
        reinterpret_cast<unsigned char *>(self) + 0x1f40)[index];
}
__forceinline const char *WaveArchivePath(SoundManagerQueueView *self)
{
    // The original opens the distinct +0x5108 archive pathname, rather
    // than the slot's source file name at +0x4108+index*0x100.
    return reinterpret_cast<const char *>(self) + 0x5108;
}
__forceinline void EnterPreparedMusicSection()
{
    if (g_MainRuntime.flags & 0x8000)
    {
        EnterCriticalSection(&g_MainRuntime.criticalSections[2]);
        ++g_MainRuntime.criticalDepth[2];
    }
}
__forceinline void LeavePreparedMusicSection()
{
    if (g_MainRuntime.flags & 0x8000)
    {
        LeaveCriticalSection(&g_MainRuntime.criticalSections[2]);
        --g_MainRuntime.criticalDepth[2];
    }
}
}

// Native TH11 00449C70..00449E72: 515-byte complete prepared BGM loader.
// The source preserves the originally unchecked index, pathname and byte
// allocation, plus the separately observed two unlock/error exit paths.
int __stdcall LoadPreparedMusic(SoundManagerQueueView *self, int index, const char *name)
{
    if (LoadedBuffer(self, index) &&
        strcmp(name, self->filenames[index]) == 0)
        return 0;

    strcpy(self->filenames[index], name);
    if (!(reinterpret_cast<const unsigned char *>(&g_MainRuntime)[0x200] & 0x10) ||
        !self->enabled)
        return 0;

    if (LoadedBuffer(self, index))
    {
        free(LoadedBuffer(self, index));
        LoadedBuffer(self, index) = 0;
    }
    TraceSoundQueue(g_PreparedMusicPreloadTrace, index);
    EnterPreparedMusicSection();

    HANDLE file = CreateFileA(WaveArchivePath(self), GENERIC_READ,
        FILE_SHARE_READ, 0, OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, 0);
    if (file == INVALID_HANDLE_VALUE)
    {
        TraceSoundQueue(g_PreparedMusicMissingFileTrace, WaveArchivePath(self));
        LeavePreparedMusicSection();
        return -1;
    }

    int track = FindSoundTrack(name, self);
    SoundTrackRecord *record = self->records + track;
    SetFilePointer(file, record->dataOffset, 0, FILE_BEGIN);
    unsigned char *data = static_cast<unsigned char *>(malloc(record->storedBytes));
    if (!data)
    {
        CloseHandle(file);
        TraceSoundQueue(g_PreparedMusicMissingFileTrace, WaveArchivePath(self));
        LeaveRuntimeCriticalSection(&g_MainRuntime, 2);
        return -1;
    }
    DWORD read;
    ReadFile(file, data, record->storedBytes, &read, 0);
    CloseHandle(file);
    LeaveRuntimeCriticalSection(&g_MainRuntime, 2);
    CachedTrack(self, index) = record;
    LoadedBuffer(self, index) = data;
    PlaybackBuffer(self, index) = data;
    CachedBytes(self, index) = CachedTrack(self, index)->storedBytes;
    return 0;
}
