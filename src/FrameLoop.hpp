#pragma once

#include "Camera.hpp"
#include "MainRuntime.hpp"

struct RuntimeThreadView
{
    unsigned int unknown00;
    HANDLE thread;
    unsigned int unknown08;
    int stopRequested;
    int pending;
    unsigned int unknown14;
    unsigned int unknown18;
};

struct FrameLoopView
{
    unsigned char unknown00[0x14];
    signed char skippedFrames;
    unsigned char unknown15[0x40 - 0x15];
    double frameStart;
    unsigned char unknown48[0x60 - 0x48];
    double lastPresent;
    double beforePresent;

    void PresentFrame();
};

// Additional observed fields at the MainRuntime root; complete ownership is open.
struct FrameRuntimeView
{
    unsigned char unknown000[0xf4];
    D3DPRESENT_PARAMETERS presentation;
    unsigned char unknown12c[0x1e6 - 0x12c];
    unsigned char frameSkip;
    unsigned char unknown1e7[4];
    unsigned char waitForPresent;
    unsigned char unknown1ec[0x31c - 0x1ec];
    CameraView screenCamera;
    unsigned char unknown40c[0x54c - 0x40c];
    CameraView *activeCamera;
    int activeCameraIndex;
    unsigned char unknown554[0x574 - 0x554];
    int presentationState;
    unsigned char unknown578[0x7f0 - 0x578];
    RuntimeThreadView worker;
    unsigned char unknown80c[0x990 - 0x80c];
    int fogEnabled;
    unsigned char unknown994[0x9b8 - 0x994];
    double frameElapsed;
};

typedef char FrameCounterOffset[(offsetof(FrameLoopView, skippedFrames) == 0x14) ? 1 : -1];
typedef char FrameStartOffset[(offsetof(FrameLoopView, frameStart) == 0x40) ? 1 : -1];
typedef char FramePresentOffset[(offsetof(FrameLoopView, lastPresent) == 0x60) ? 1 : -1];
typedef char FrameRuntimeSkipOffset[(offsetof(FrameRuntimeView, frameSkip) == 0x1e6) ? 1 : -1];
typedef char FrameRuntimeWaitOffset[(offsetof(FrameRuntimeView, waitForPresent) == 0x1eb) ? 1 : -1];
typedef char FrameRuntimeCameraOffset[(offsetof(FrameRuntimeView, screenCamera) == 0x31c) ? 1 : -1];
typedef char FrameRuntimeActiveOffset[(offsetof(FrameRuntimeView, activeCamera) == 0x54c) ? 1 : -1];
typedef char FrameRuntimeStateOffset[(offsetof(FrameRuntimeView, presentationState) == 0x574) ? 1 : -1];
typedef char FrameRuntimeWorkerOffset[(offsetof(FrameRuntimeView, worker) == 0x7f0) ? 1 : -1];
typedef char FrameRuntimeFogOffset[(offsetof(FrameRuntimeView, fogEnabled) == 0x990) ? 1 : -1];
typedef char FrameRuntimeElapsedOffset[(offsetof(FrameRuntimeView, frameElapsed) == 0x9b8) ? 1 : -1];

inline FrameRuntimeView &FrameRuntime()
{
    return *reinterpret_cast<FrameRuntimeView *>(&g_MainRuntime);
}

void __stdcall StopRuntimeThread(RuntimeThreadView *self);
HRESULT __stdcall DisableRuntimeFog(MainRuntimeView *self);
int __stdcall RunFrame(FrameLoopView *self);

// Independently reviewed zero-input reset and post-present call boundaries.
void __cdecl ReleaseRuntimeSurfaces();
void __cdecl PrepareRendererForReset();
void __cdecl RestoreRendererAfterReset();
void __stdcall EnsureRuntimeSurfaces(MainRuntimeView *self);
void __cdecl ResetDeviceRenderStates();
void __cdecl UpdatePostPresentClock();
void __cdecl UpdatePostPresentAudio();
extern void *g_PostPresentClock;
extern void *g_PostPresentAudio;
