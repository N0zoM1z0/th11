#pragma once

#include <windows.h>
#include <stddef.h>

struct SoundBufferView;
// Only observed DirectSound COM calls are typed; unknown slots are never invoked.
struct SoundBufferVtable
{
    void *unknown000[12];
    HRESULT (WINAPI *play)(SoundBufferView *, DWORD, DWORD, DWORD);
    HRESULT (WINAPI *setCurrentPosition)(SoundBufferView *, DWORD);
    void *unknown038;
    HRESULT (WINAPI *setVolume)(SoundBufferView *, LONG);
    HRESULT (WINAPI *setPan)(SoundBufferView *, LONG);
    void *unknown044;
    HRESULT (WINAPI *stop)(SoundBufferView *);
};
struct SoundBufferView
{
    SoundBufferVtable *vtable;
    __forceinline HRESULT Play(DWORD reserved, DWORD priority, DWORD flags)
    { return vtable->play(this, reserved, priority, flags); }
    __forceinline HRESULT SetCurrentPosition(DWORD position)
    { return vtable->setCurrentPosition(this, position); }
    __forceinline HRESULT SetVolume(LONG volume)
    { return vtable->setVolume(this, volume); }
    __forceinline HRESULT SetPan(LONG pan)
    { return vtable->setPan(this, pan); }
    __forceinline HRESULT Stop() { return vtable->stop(this); }
};
typedef char SoundBufferPlayOffset[(offsetof(SoundBufferVtable, play) == 0x30) ? 1 : -1];
typedef char SoundBufferStopOffset[(offsetof(SoundBufferVtable, stop) == 0x48) ? 1 : -1];
