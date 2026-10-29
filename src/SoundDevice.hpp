#pragma once

#include "SoundBuffer.hpp"

#pragma pack(push, 2)
struct SoundWaveFormatView
{
    WORD formatTag, channels;
    DWORD samplesPerSecond, bytesPerSecond;
    WORD blockAlign, bitsPerSample, extraBytes;
};
#pragma pack(pop)

struct SoundBufferDescription
{
    DWORD size, flags, bytes, reserved;
    SoundWaveFormatView *format;
    GUID algorithm;
};
struct SoundNotificationPosition
{
    DWORD offset;
    HANDLE event;
};
struct SoundDeviceCapabilities;
struct SoundUnknownView;

// Standard COM protocols; implementations remain with the sound device.
struct __declspec(novtable) SoundDeviceView
{
    virtual HRESULT WINAPI QueryInterface(const GUID &id, void **object) = 0;
    virtual ULONG WINAPI AddRef() = 0;
    virtual ULONG WINAPI Release() = 0;
    virtual HRESULT WINAPI CreateSoundBuffer(const SoundBufferDescription *description,
        SoundBufferView **buffer, SoundUnknownView *outer) = 0;
    virtual HRESULT WINAPI GetCaps(SoundDeviceCapabilities *caps) = 0;
    virtual HRESULT WINAPI DuplicateSoundBuffer(SoundBufferView *original, SoundBufferView **copy) = 0;
    virtual HRESULT WINAPI SetCooperativeLevel(HWND window, DWORD level) = 0;
    virtual HRESULT WINAPI Compact() = 0;
    virtual HRESULT WINAPI GetSpeakerConfig(DWORD *config) = 0;
    virtual HRESULT WINAPI SetSpeakerConfig(DWORD config) = 0;
    virtual HRESULT WINAPI Initialize(const GUID *device) = 0;
    virtual HRESULT WINAPI VerifyCertification(DWORD *certified) = 0;
};
struct __declspec(novtable) SoundNotificationView
{
    virtual HRESULT WINAPI QueryInterface(const GUID &id, void **object) = 0;
    virtual ULONG WINAPI AddRef() = 0;
    virtual ULONG WINAPI Release() = 0;
    virtual HRESULT WINAPI SetNotificationPositions(DWORD count,
        const SoundNotificationPosition *positions) = 0;
};
struct SoundDeviceOwnerView { SoundDeviceView *device; };

typedef char SoundWaveFormatSize[(sizeof(SoundWaveFormatView) == 18) ? 1 : -1];
typedef char SoundBufferDescriptionSize[(sizeof(SoundBufferDescription) == 36) ? 1 : -1];
typedef char SoundNotificationPositionSize[(sizeof(SoundNotificationPosition) == 8) ? 1 : -1];
