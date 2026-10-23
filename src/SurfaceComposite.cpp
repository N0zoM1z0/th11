#include "SurfaceComposite.hpp"

bool SurfaceCompositeView::BlendToSurface(IDirect3DSurface9 *surface,
    int x, int y, int copyWidth, int copyHeight)
{
    if (!hasBuffer)
        return false;
    D3DSURFACE_DESC description;
    surface->GetDesc(&description);
    RECT area;
    area.left = 0;
    area.top = 0;
    area.right = width;
    area.bottom = height;
    D3DLOCKED_RECT locked;
    if (surface->LockRect(&locked, &area, 0) != D3D_OK)
        return false;
    unsigned int sourcePitch = rowBytes;
    unsigned char *source = pixels;
    int destinationPitch = locked.Pitch;
    if (description.Format == format)
    {
        switch (format)
        {
        case D3DFMT_A8R8G8B8:
        {
            unsigned char *destination = static_cast<unsigned char *>(locked.pBits)
                + destinationPitch * y + x * 4;
            for (int row = 0; row < copyHeight; ++row)
            {
                int column;
                for (column = 0; column < copyWidth; ++column)
                {
                    int alpha = source[3];
                    destination[0] += ((source[0] - destination[0]) * alpha) >> 8;
                    destination[1] += ((source[1] - destination[1]) * alpha) >> 8;
                    destination[2] += ((source[2] - destination[2]) * alpha) >> 8;
                    destination += 4;
                    source += 4;
                }
                source += sourcePitch - column * 4;
                destination += destinationPitch - column * 4;
            }
            break;
        }
        case D3DFMT_A1R5G5B5:
        {
            unsigned char *destination = static_cast<unsigned char *>(locked.pBits)
                + destinationPitch * y + x * 2;
            for (int row = 0; row < copyHeight; ++row)
            {
                int column;
                for (column = 0; column < copyWidth; ++column)
                {
                    unsigned short pixel = *reinterpret_cast<unsigned short *>(source);
                    if (pixel & 0x8000)
                        *reinterpret_cast<unsigned short *>(destination) = pixel;
                    destination += 2;
                    source += 2;
                }
                source += sourcePitch - column * 2;
                destination += destinationPitch - column * 2;
            }
            break;
        }
        case D3DFMT_A4R4G4B4:
        {
            unsigned char *destination = static_cast<unsigned char *>(locked.pBits)
                + destinationPitch * y + x * 2;
            for (int row = 0; row < copyHeight; ++row)
            {
                int column;
                for (column = 0; column < copyWidth; ++column)
                {
                    int alpha = source[1] >> 4;
                    destination[0] = ((((source[0] >> 4) - (destination[0] >> 4))
                        * alpha >> 4) + (destination[0] >> 4)) << 4;
                    destination[0] |= (((source[0] & 15) - (destination[0] & 15))
                        * alpha >> 4) + (destination[0] & 15);
                    int outputAlpha = ((destination[1] >> 2) & 0x3c) + alpha;
                    if (outputAlpha >= 16)
                        outputAlpha = 15;
                    destination[1] = ((((source[1] & 15) - (destination[1] & 15))
                        * alpha >> 4) + (destination[1] & 15)) | (outputAlpha << 4);
                    destination += 2;
                    source += 2;
                }
                source += sourcePitch - column * 2;
                destination += destinationPitch - column * 2;
            }
            break;
        }
        default:
            return false;
        }
    }
    surface->UnlockRect();
    return true;
}
