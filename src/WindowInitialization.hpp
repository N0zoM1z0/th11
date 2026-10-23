#pragma once

#include "MainRuntime.hpp"
#include "MainWindow.hpp"

union WindowInitializationFlags
{
    unsigned int value;
    struct { unsigned int unknown0 : 2, mode : 2, unknown4 : 28; } bits;
};

struct WindowInitializationTriplet
{
    int value0, value4, value8;
};

// Additional physical fields at the shared runtime root; full ownership is open.
struct WindowInitializationView
{
    unsigned char unknown000[0x10];
    RECT windowRect;
    unsigned char unknown020[0x58 - 0x20];
    HWND windowAlias;
    unsigned char unknown05c[0x114 - 0x5c];
    BOOL windowed;
    unsigned char unknown118[0x1e5 - 0x118];
    unsigned char displayMode;
    unsigned char frameSkip;
    unsigned char unknown1e7[4];
    unsigned char timingMode;
    unsigned char unknown1ec[4];
    int windowX, windowY;
    unsigned char unknown1f8[0xb08 - 0x1f8];
    HWND mainWindow;
    unsigned char unknownb0c[8];
    unsigned int active, inactive;
    unsigned char unknownb1c[0xb40 - 0xb1c];
    WindowInitializationFlags flags;
    unsigned char unknownb44[0xb80 - 0xb44];
    int resetb80;
    WindowInitializationTriplet triplets[4];
};

typedef char WindowInitModeOffset[(offsetof(WindowInitializationView, displayMode) == 0x1e5) ? 1 : -1];
typedef char WindowInitHandleOffset[(offsetof(WindowInitializationView, mainWindow) == 0xb08) ? 1 : -1];
typedef char WindowInitFlagsOffset[(offsetof(WindowInitializationView, flags) == 0xb40) ? 1 : -1];
typedef char WindowInitTripletsOffset[(offsetof(WindowInitializationView, triplets) == 0xb84) ? 1 : -1];

extern const char g_MainWindowClassName[];
extern const char g_MainWindowTitle[];

int __stdcall InitializeMainWindow(HINSTANCE instance);
