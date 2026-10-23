#include "Screenshot.hpp"
#include "InputState.hpp"
#include "FileSystem.hpp"

#include <stdio.h>
#include <direct.h>

extern InputStateView g_InputState;

void __stdcall HandleScreenshotHotkey()
{
    char filename[MAX_PATH];
    FlushScreenshotResources();
    if (g_InputState.pressed & 0x40000)
    {
        _mkdir("snapshot");
        int index;
        for (index = 0; index < 1000; ++index)
        {
            sprintf(filename, "snapshot/th%.3d.bmp", index);
            if (!FileExists(filename))
                break;
        }
        if (index < 1000)
            CaptureScreenshot(filename);
    }
}
