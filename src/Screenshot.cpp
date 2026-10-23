#include "Screenshot.hpp"

#include <process.h>
#include <stdlib.h>
#include <string.h>

int __stdcall CaptureScreenshot(const char *filename)
{
    ScreenshotStateView &capture = ScreenshotState();
    while (capture.thread != 0)
        Sleep(10);

    IDirect3DSurface9 *surface = 0;
    ScreenshotDebugPrint("SnapShot! %s\n", filename);
    static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice)->GetBackBuffer(
        0, 0, D3DBACKBUFFER_TYPE_MONO, &surface);

    memset(&capture.fileHeader, 0, sizeof(capture.fileHeader));
    capture.fileHeader.bfOffBits = 54;
    capture.fileHeader.bfSize = 54;
    capture.fileHeader.bfType = 0x4d42;
    strcpy(capture.filename, filename);

    switch (PresentationParameters().BackBufferFormat)
    {
    default:
        AppendLog(&g_ErrorLog, "error ? .\\src\\game\\mother.cpp\r\n");
        return 1;

    case D3DFMT_R5G6B5:
        AppendLog(&g_ErrorLog, "16bit \x82\xcd\x8e\xe6\x82\xe8\x8d\x9e\x82\xdf\x82\xc8\x82\xa2\r\n");
        break;

    case D3DFMT_X8R8G8B8:
        capture.info = static_cast<BITMAPINFO *>(malloc(sizeof(BITMAPINFO)));
        if (!capture.info)
            goto allocation_failed;
        memset(capture.info, 0, sizeof(BITMAPINFO));
        capture.pixels = static_cast<unsigned char *>(malloc(640 * 480 * 3));
        if (!capture.pixels)
        {
        allocation_failed:
            AppendLog(&g_ErrorLog, "snapShotScreen : \x8a\x6d\x95\xdb\x82\xb5\x82\xad\x82\xe8\r\n");
            break;
        }
        {
            capture.fileHeader.bfSize += 640 * 480 * 3;
            capture.info->bmiHeader.biBitCount = 24;
            capture.info->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
            capture.info->bmiHeader.biWidth = 640;
            capture.info->bmiHeader.biHeight = 480;
            capture.info->bmiHeader.biPlanes = 1;
            capture.info->bmiHeader.biCompression = BI_RGB;

            D3DLOCKED_RECT locked;
            surface->LockRect(&locked, 0, 0);
            int y = 479;
            unsigned int destinationOffset = 0;
            do
            {
                const unsigned char *source = static_cast<unsigned char *>(locked.pBits) + locked.Pitch * y;
                unsigned char *destination = capture.pixels + destinationOffset;
                int remaining = 640;
                do
                {
                    memcpy(destination, source, 3);
                    source += 4;
                    destination += 3;
                } while (--remaining != 0);
                --y;
                destinationOffset += 640 * 3;
            } while (y > -1);
            surface->UnlockRect();
            capture.thread = _beginthread(ScreenshotWriteThread, 0, 0);
            break;
        }
    }

    if (surface)
        surface->Release();
    return 0;
}
