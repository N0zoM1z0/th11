#pragma once

#include <windows.h>

// Opaque pointees for unused parts of the DirectSound buffer protocol.
struct SoundBufferCapabilities;
struct SoundBufferDescription;
struct SoundDeviceView;
struct SoundWaveFormatView;

// Complete COM method order. Implementations belong to the actual device.
struct __declspec(novtable) SoundBufferView
{
    virtual HRESULT WINAPI QueryInterface(const GUID &id, void **object) = 0;
    virtual ULONG WINAPI AddRef() = 0;
    virtual ULONG WINAPI Release() = 0;
    virtual HRESULT WINAPI GetCaps(SoundBufferCapabilities *caps) = 0;
    virtual HRESULT WINAPI GetCurrentPosition(DWORD *play, DWORD *write) = 0;
    virtual HRESULT WINAPI GetFormat(SoundWaveFormatView *format, DWORD capacity, DWORD *written) = 0;
    virtual HRESULT WINAPI GetVolume(LONG *volume) = 0;
    virtual HRESULT WINAPI GetPan(LONG *pan) = 0;
    virtual HRESULT WINAPI GetFrequency(DWORD *frequency) = 0;
    virtual HRESULT WINAPI GetStatus(DWORD *status) = 0;
    virtual HRESULT WINAPI Initialize(SoundDeviceView *device, const SoundBufferDescription *description) = 0;
    virtual HRESULT WINAPI Lock(DWORD offset, DWORD bytes, void **first, DWORD *firstBytes,
        void **second, DWORD *secondBytes, DWORD flags) = 0;
    virtual HRESULT WINAPI Play(DWORD reserved, DWORD priority, DWORD flags) = 0;
    virtual HRESULT WINAPI SetCurrentPosition(DWORD position) = 0;
    virtual HRESULT WINAPI SetFormat(const SoundWaveFormatView *format) = 0;
    virtual HRESULT WINAPI SetVolume(LONG volume) = 0;
    virtual HRESULT WINAPI SetPan(LONG pan) = 0;
    virtual HRESULT WINAPI SetFrequency(DWORD frequency) = 0;
    virtual HRESULT WINAPI Stop() = 0;
    virtual HRESULT WINAPI Unlock(void *first, DWORD firstBytes, void *second, DWORD secondBytes) = 0;
    virtual HRESULT WINAPI Restore() = 0;
};
typedef char SoundBufferInterfaceSize[(sizeof(SoundBufferView) == 4) ? 1 : -1];
