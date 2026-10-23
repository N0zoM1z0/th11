#pragma once

#include "Camera.hpp"
#include "MainRuntime.hpp"

// Observed fields only; this span does not establish the original owner size.
struct DeviceInitializationView
{
    HINSTANCE instance;
    IDirect3D9 *d3d;
    IDirect3DDevice9 *device;
    unsigned char unknown00c[0x5c - 0xc];
    D3DMATRIX view;
    D3DMATRIX projection;
    D3DVIEWPORT9 viewport;
    D3DPRESENT_PARAMETERS presentation;
    unsigned char unknown12c[0x19c - 0x12c];
    D3DDISPLAYMODE desktop;
    unsigned char unknown1ac[0x1e2 - 0x1ac];
    unsigned char colorMode;
    unsigned char unknown1e3[2];
    unsigned char displayMode;
    unsigned char unknown1e6[5];
    unsigned char timingMode;
    unsigned char unknown1ec[0x200 - 0x1ec];
    unsigned int compatibilityOptions;
    unsigned char unknown204[0x57c - 0x204];
    int disableRefreshChange;
    int refreshChangeActive;
    int reset584;
    unsigned char unknown588[8];
    unsigned int flags;
    unsigned char unknown594[8];
    D3DCAPS9 caps;
    unsigned char unknown6cc[0xb08 - 0x6cc];
    HWND window;
    unsigned int resetb0c;
    unsigned char unknownb10[0xb30 - 0xb10];
    unsigned char pathMismatch;
    unsigned char unknownb31[0xb40 - 0xb31];
    unsigned int presentationFlags;
};

typedef char DeviceViewMatrixOffset[(offsetof(DeviceInitializationView, view) == 0x5c) ? 1 : -1];
typedef char DeviceViewPresentationOffset[(offsetof(DeviceInitializationView, presentation) == 0xf4) ? 1 : -1];
typedef char DeviceViewDesktopOffset[(offsetof(DeviceInitializationView, desktop) == 0x19c) ? 1 : -1];
typedef char DeviceViewOptionsOffset[(offsetof(DeviceInitializationView, compatibilityOptions) == 0x200) ? 1 : -1];
typedef char DeviceViewCapsOffset[(offsetof(DeviceInitializationView, caps) == 0x59c) ? 1 : -1];
typedef char DeviceViewWindowOffset[(offsetof(DeviceInitializationView, window) == 0xb08) ? 1 : -1];

inline DeviceInitializationView &DeviceRuntime()
{
    return *reinterpret_cast<DeviceInitializationView *>(&g_MainRuntime);
}

extern const char g_DeviceLogRefreshNot60[], g_DeviceLogInitialColor[];
extern const char g_DeviceLogTry60[], g_DeviceLogTryImmediate[];
extern const char g_DeviceLogHardwareUnavailable[], g_DeviceLogHalUnavailable[];
extern const char g_DeviceLogRefreshFailed[], g_DeviceLogCreationFailed[];
extern const char g_DeviceLogReference[], g_DeviceLogSoftware[], g_DeviceLogHardware[];
extern const char g_DeviceLogNoAdd[], g_DeviceLogSmallTextures[], g_DeviceLogReducedColor[];

int __cdecl InitializeD3DDevice();
void __cdecl ResetMainViewport();
void __cdecl ResetDeviceRenderStates();
void __cdecl PrimeDeviceBackBuffers();
