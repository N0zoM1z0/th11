#include "SurfaceBuffer.hpp"
#include <string.h>

bool SurfaceBufferView::ReadFromSurface(IDirect3DSurface9 *surface)
{
    if (!hasBuffer)
        return false;
    D3DSURFACE_DESC description;
    D3DLOCKED_RECT locked;
    surface->GetDesc(&description);
    RECT area = {0, 0, width, height};
    surface->LockRect(&locked, &area, D3DLOCK_READONLY);
    unsigned int bytes = rowBytes;
    unsigned char *destination = pixels;
    unsigned char *source = static_cast<unsigned char *>(locked.pBits);
    int pitch = locked.Pitch;
    if (description.Format == format)
    {
        for (int row = 0; row < height; ++row)
        {
            memcpy(destination, source, bytes);
            source += pitch;
            destination += bytes;
        }
    }
    surface->UnlockRect();
    return true;
}

bool SurfaceBufferView::WriteToSurface(IDirect3DSurface9 *surface)
{
    if (!hasBuffer)
        return false;
    D3DSURFACE_DESC description;
    D3DLOCKED_RECT locked;
    surface->GetDesc(&description);
    RECT area = {0, 0, width, height};
    if (surface->LockRect(&locked, &area, 0) != D3D_OK)
        return false;
    unsigned char *source = pixels;
    unsigned char *destination = static_cast<unsigned char *>(locked.pBits);
    unsigned int bytes = rowBytes;
    int pitch = locked.Pitch;
    if (description.Format == format)
    {
        for (int row = 0; row < height; ++row)
        {
            memcpy(destination, source, bytes);
            destination += pitch;
            source += bytes;
        }
    }
    surface->UnlockRect();
    return true;
}
