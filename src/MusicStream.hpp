#pragma once

#include "SoundQueue.hpp"

extern DWORD g_MusicArchiveOffset;
extern const char g_MusicOpenTrace[], g_MusicConfigureTrace[];

HRESULT __stdcall RestoreMusicBuffer(SoundBufferView *buffer, BOOL *restored);
SoundBufferView *__stdcall SelectAvailableMusicBuffer(MusicControllerView *self);
HRESULT __stdcall RefillMusicBuffer(MusicControllerView *self, SoundBufferView *buffer, int loop);
HRESULT __stdcall ResetMusicBufferPositions(MusicControllerView *self);
HRESULT __stdcall PlayMusicStream(MusicControllerView *self);
HRESULT __stdcall StopMusicStream(MusicControllerView *self, int closeFile);
HRESULT __stdcall PauseMusicStream(MusicControllerView *self);
HRESULT __stdcall ResumeMusicStream(MusicControllerView *self);
HRESULT __stdcall UpdateMusicStream(MusicControllerView *self);
HRESULT __stdcall ResetMusicPosition(MusicControllerView *self);
HRESULT __stdcall OpenMusicDecoderFile(MusicDecoderView *self, const char *filename, SoundTrackRecord *record);
HRESULT __stdcall ConfigureMusicDecoder(MusicDecoderView *self, SoundTrackRecord *record, int offset);
HRESULT __stdcall ResetMusicDecoder(MusicDecoderView *self, DWORD offset, bool loop);
HRESULT __stdcall ReadMusicDecoder(MusicDecoderView *self, void *destination, DWORD bytes, DWORD *read);

extern const GUID g_MusicBufferInterfaceId;
HRESULT __stdcall QueryMusicBufferInterface(MusicControllerView *self, DWORD index, void **object);
BOOL __stdcall IsAnyMusicBufferPlaying(MusicControllerView *self);
int __stdcall StepMusicFadeMode1(MusicControllerView *self);
int __stdcall StepMusicFadeMode2(MusicControllerView *self);
int __stdcall StepMusicFadeMode3(MusicControllerView *self);
int __stdcall StepMusicFadeMode4(MusicControllerView *self);
HRESULT __stdcall BindMusicDecoderMemory(MusicDecoderView *self, SoundTrackRecord *record,
    const unsigned char *data, DWORD bytes);
DWORD __stdcall MusicDecoderDataSize(MusicDecoderView *self);
HRESULT __stdcall CloseMusicDecoder(MusicDecoderView *self);
DWORD __stdcall MusicDecoderSavedPosition(MusicDecoderView *self);
HRESULT __stdcall ConfigureControllerMusicDecoder(MusicControllerView *self, SoundTrackRecord *record, int offset);
void __stdcall SaveMusicDecoderPosition(MusicDecoderView *self);
