#pragma once

#include "MainRuntime.hpp"
#include "AnimationVm.hpp"

struct RuntimeCameraRecord
{
    unsigned char storage[0x118];
};

struct RuntimeSurfaceOwnerView
{
    unsigned char unknown000[8];
    IDirect3DDevice9 *device;
    unsigned char unknown00c[0x1ac - 0xc];
    IDirect3DSurface9 *primarySurface;
    IDirect3DSurface9 *secondarySurface;
    IDirect3DSurface9 *backBuffer;
    unsigned int unknown1b8;
    AnimationVmView *primaryVm;
    AnimationVmView *secondaryVm;
    AnimationVmView *overlayVm;
    unsigned char unknown1c8[0x204 - 0x1c8];
    RuntimeCameraRecord cameras[3];
    unsigned char unknown54c[0x588 - 0x54c];
    AnimationFileView *animation;
};

typedef char RuntimeSurfacePrimaryOffset[(offsetof(RuntimeSurfaceOwnerView, primarySurface) == 0x1ac) ? 1 : -1];
typedef char RuntimeSurfaceVmOffset[(offsetof(RuntimeSurfaceOwnerView, primaryVm) == 0x1bc) ? 1 : -1];
typedef char RuntimeSurfaceCameraOffset[(offsetof(RuntimeSurfaceOwnerView, cameras) == 0x204) ? 1 : -1];
typedef char RuntimeSurfaceAnimationOffset[(offsetof(RuntimeSurfaceOwnerView, animation) == 0x588) ? 1 : -1];

void __stdcall EnsureRuntimeSurfaces(MainRuntimeView *self);
void __cdecl ReleaseRuntimeSurfaces();
