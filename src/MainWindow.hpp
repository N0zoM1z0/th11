#pragma once

#include "MainRuntime.hpp"

// Additional fields observed at the shared runtime root. Full ownership is open.
struct WindowRuntimeView
{
    unsigned char unknown000[0x114];
    BOOL windowed;
    unsigned char unknown118[0xb14 - 0x118];
    WPARAM active;
    BOOL inactive;
    unsigned char unknownb1c[0xb40 - 0xb1c];
    unsigned int resizeFlags;
};

typedef char WindowActiveOffset[(offsetof(WindowRuntimeView, active) == 0xb14) ? 1 : -1];
typedef char WindowResizeOffset[(offsetof(WindowRuntimeView, resizeFlags) == 0xb40) ? 1 : -1];

LRESULT CALLBACK MainWindowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam);
