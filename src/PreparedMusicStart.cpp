#include "SoundQueue.hpp"
#include "MusicStream.hpp"
#include "MainRuntime.hpp"

// Observed original 0x449C20 file-backed playback entry and no-op trace
// (0x44AB00). Their independent source owners are still open.
extern int __stdcall StartPreparedMusicFile(const char *filename);
extern void __cdecl TraceSoundQueue(const char *format, ...);
extern const char g_PreparedMusicStartTrace[];
extern const GUID g_PreparedMusicStreamAlgorithm;

namespace
{
__forceinline unsigned char PreparedRuntimeByte(unsigned int offset)
{
    return reinterpret_cast<const unsigned char *>(&g_MainRuntime)[offset];
}
__forceinline SoundTrackRecord *PreparedRecord(SoundManagerQueueView *self, int index)
{
    return reinterpret_cast<SoundTrackRecord **>(
        reinterpret_cast<unsigned char *>(self) + 0x1e80)[index];
}
__forceinline unsigned char *PreparedAudioBytes(SoundManagerQueueView *self, int index)
{
    return reinterpret_cast<unsigned char **>(
        reinterpret_cast<unsigned char *>(self) + 0x1f00)[index];
}
__forceinline DWORD PreparedAudioSize(SoundManagerQueueView *self, int index)
{
    return reinterpret_cast<DWORD *>(
        reinterpret_cast<unsigned char *>(self) + 0x1f40)[index];
}
__forceinline int &PreparedPlayingIndex(SoundManagerQueueView *self)
{
    return *reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(self) + 0x1f80);
}
}

// Native 0x449E80..0x449FA2: start one prepared stream or dispatch file mode.
int __stdcall StartPreparedMusic(SoundManagerQueueView *self, int index)
{
    if (!self->enabled || !PreparedRuntimeByte(0x1e3) ||
        !reinterpret_cast<SoundDeviceOwnerView *>(self)->device)
        return -1;

    if (!(PreparedRuntimeByte(0x200) & 0x10))
        return StartPreparedMusicFile(self->filenames[index]);

    if (!reinterpret_cast<unsigned char **>(
        reinterpret_cast<unsigned char *>(self) + 0x1ec0)[index])
        return -1;

    TraceSoundQueue(g_PreparedMusicStartTrace, index);
    SoundTrackRecord *record = PreparedRecord(self, index);
    unsigned int blockAlign = record->blockAlign;
    unsigned int durationBytes = (record->samplesPerSecond * blockAlign & 0x3fffffffu) >> 2;
    g_MusicEvent = CreateEventA(0, FALSE, FALSE, 0);
    self->thread = CreateThread(0, 0, MusicStreamingThread,
        *reinterpret_cast<void **>(reinterpret_cast<unsigned char *>(&g_MainRuntime) + 0x58),
        0, &self->threadId);
    durationBytes -= durationBytes % blockAlign;
    SoundDeviceOwnerView *owner = reinterpret_cast<SoundDeviceOwnerView *>(self->enabled);
    HRESULT result = owner->CreateMemoryMusicStream(&g_MusicController,
        PreparedAudioBytes(self, index), PreparedAudioSize(self, index), record,
        g_PreparedMusicStreamAlgorithm, durationBytes, g_MusicEvent);
    if (result < 0) return -1;
    PreparedPlayingIndex(self) = index;
    return 0;
}
