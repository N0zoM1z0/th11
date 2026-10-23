#include "Screenshot.hpp"
#include "SharedFile.hpp"

#include <stdlib.h>

void __cdecl ScreenshotWriteThread(void *)
{
    ScreenshotStateView &capture = ScreenshotState();
    OpenSharedFileForWrite(capture.filename);
    if (g_SharedFile != INVALID_HANDLE_VALUE)
    {
        DWORD written;
        WriteFile(g_SharedFile, &capture.fileHeader, sizeof(capture.fileHeader), &written, 0);
        if (written != sizeof(capture.fileHeader))
        {
            CloseHandle(g_SharedFile);
            UnlockSharedFile();
        }
    }

    WriteSharedFile(capture.info, sizeof(BITMAPINFOHEADER));
    WriteSharedFile(capture.pixels, 640 * 480 * 3);
    CloseSharedFile();

    if (capture.info)
    {
        free(capture.info);
        capture.info = 0;
    }
    if (capture.pixels)
    {
        free(capture.pixels);
        capture.pixels = 0;
    }
    capture.thread = 0;
}
