// Independent buffers/decoder/controller and output cells; never executed.
#include "MusicStream.hpp"
extern "C" HRESULT __stdcall MusicStreamProbe(int kind, MusicControllerView *music,
    SoundBufferView *buffer, MusicDecoderView *decoder, BOOL *restored,
    void *data, DWORD bytes, DWORD *read, SoundTrackRecord *record,
    const char *filename, int setting)
{
    switch (kind)
    {
    case 0: return RestoreMusicBuffer(buffer, restored);
    case 1: return reinterpret_cast<HRESULT>(SelectAvailableMusicBuffer(music));
    case 2: return RefillMusicBuffer(music, buffer, setting);
    case 3: return ResetMusicBufferPositions(music);
    case 4: return PlayMusicStream(music);
    case 5: return StopMusicStream(music, setting);
    case 6: return PauseMusicStream(music);
    case 7: return ResumeMusicStream(music);
    case 8: return UpdateMusicStream(music);
    case 9: return ResetMusicPosition(music);
    case 10: return OpenMusicDecoderFile(decoder, filename, record);
    case 11: return ConfigureMusicDecoder(decoder, record, setting);
    case 12: return ResetMusicDecoder(decoder, bytes, setting != 0);
    case 13: return ReadMusicDecoder(decoder, data, bytes, read);
    case 14: return QueryMusicBufferInterface(music, bytes, reinterpret_cast<void **>(data));
    case 15: return IsAnyMusicBufferPlaying(music);
    case 16: return StepMusicFadeMode1(music);
    case 17: return StepMusicFadeMode2(music);
    case 18: return StepMusicFadeMode3(music);
    case 19: return StepMusicFadeMode4(music);
    case 20: return BindMusicDecoderMemory(decoder, record, static_cast<const unsigned char *>(data), bytes);
    case 21: return MusicDecoderDataSize(decoder);
    case 22: return CloseMusicDecoder(decoder);
    case 23: return MusicDecoderSavedPosition(decoder);
    case 24: return ConfigureControllerMusicDecoder(music, record, setting);
    case 25: SaveMusicDecoderPosition(decoder); break;
    case 26: return RecreateMusicBuffers(music);
    case 27: return ConfigurePrimarySoundFormat(reinterpret_cast<SoundDeviceOwnerView *>(data));
    case 28: InitializeSoundDeviceOwner(reinterpret_cast<SoundDeviceOwnerView *>(data)); break;
    case 29: ReleaseSoundDevice(reinterpret_cast<SoundDeviceOwnerView *>(data)); break;
    case 31: return reinterpret_cast<SoundDeviceOwnerView *>(buffer)->CreateFileMusicStream(
        reinterpret_cast<MusicControllerView **>(read), *reinterpret_cast<const GUID *>(data),
        bytes, reinterpret_cast<HANDLE>(music), record);
    case 32: return reinterpret_cast<SoundDeviceOwnerView *>(buffer)->CreateMemoryMusicStream(
        reinterpret_cast<MusicControllerView **>(read), static_cast<const unsigned char *>(data),
        bytes, record, *reinterpret_cast<const GUID *>(filename), bytes, reinterpret_cast<HANDLE>(music));
    case 30: return InitializeSoundDevice(reinterpret_cast<SoundDeviceOwnerView *>(data), reinterpret_cast<HWND>(buffer));
    }
    return 0;
}
