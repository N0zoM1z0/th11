#pragma once

#include "MainRuntime.hpp"
#include "LogBuffer.hpp"
#include <d3d9.h>

// Views into MainRuntime's still-opaque storage, not separate data owners.
struct ScreenshotStateView
{
    unsigned int thread;
    BITMAPFILEHEADER fileHeader;
    unsigned char unknown12[2];
    BITMAPINFO *info;
    unsigned char *pixels;
    char filename[MAX_PATH];
};

typedef char ScreenshotInfoOffset[(offsetof(ScreenshotStateView, info) == 0x14) ? 1 : -1];
typedef char ScreenshotFilenameOffset[(offsetof(ScreenshotStateView, filename) == 0x1c) ? 1 : -1];
typedef char ScreenshotStateViewSize[(sizeof(ScreenshotStateView) == 0x120) ? 1 : -1];

inline ScreenshotStateView &ScreenshotState()
{
    return *reinterpret_cast<ScreenshotStateView *>(g_MainRuntime.unknown594 + 0x13c);
}

inline D3DPRESENT_PARAMETERS &PresentationParameters()
{
    return *reinterpret_cast<D3DPRESENT_PARAMETERS *>(g_MainRuntime.unknown00c + 0xe8);
}

extern LogBufferView g_ErrorLog;
void __cdecl ScreenshotDebugPrint(const char *format, ...);
void __cdecl ScreenshotWriteThread(void *context);
int __stdcall CaptureScreenshot(const char *filename);
void __stdcall FlushScreenshotResources();
void __stdcall HandleScreenshotHotkey();
