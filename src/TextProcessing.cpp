#include "TextDrawing.hpp"

union TextPixel16
{
    unsigned short value;
    struct
    {
        unsigned short blue : 5;
        unsigned short green : 5;
        unsigned short red : 5;
        unsigned short alpha : 1;
    } a1;
};

bool __stdcall InvertTextAlpha(int rows)
{
    int bytes = g_TextSurface.pitch * rows;
    unsigned char *pixels = static_cast<unsigned char *>(g_TextSurface.pixels);
    switch (g_TextSurface.format)
    {
    case D3DFMT_A8R8G8B8:
        for (int i = 3; i < bytes; i += 4)
            pixels[i] = ~pixels[i];
        break;
    case D3DFMT_A1R5G5B5:
        for (int i = 0; i < bytes; i += 2)
        {
            TextPixel16 *pixel = reinterpret_cast<TextPixel16 *>(pixels + i);
            pixel->a1.alpha = ~pixel->a1.alpha;
            if (!pixel->a1.alpha)
                pixel->value &= 0x8000;
        }
        break;
    case D3DFMT_A4R4G4B4:
        for (int i = 1; i < bytes; i += 2)
            pixels[i] ^= 0xf0;
        break;
    default:
        return false;
    }
    return true;
}

bool __stdcall FillTransparentTextPixels(unsigned int rows)
{
    if (g_TextSurface.format == D3DFMT_A8R8G8B8)
    {
        unsigned char *pixel = static_cast<unsigned char *>(g_TextSurface.pixels);
        for (unsigned int row = 0; row < rows; ++row)
        {
            for (unsigned int column = 0; column < static_cast<unsigned int>(g_TextSurface.width); ++column)
            {
                if (!pixel[3])
                {
                    unsigned int red = 0, green = 0, blue = 0, count = 0;
                    if (column && pixel[-1])
                    {
                        red = pixel[-2]; green = pixel[-3]; blue = pixel[-4];
                        count = 1;
                    }
                    if (column < static_cast<unsigned int>(g_TextSurface.width) - 1 && pixel[7])
                    {
                        red += pixel[6]; green += pixel[5]; blue += pixel[4];
                        ++count;
                    }
                    if (row)
                    {
                        unsigned char *above = pixel - (g_TextSurface.pitch / 4) * 4;
                        if (above[3])
                        {
                            red += above[2]; green += above[1]; blue += above[0];
                            ++count;
                        }
                    }
                    if (row < static_cast<unsigned int>(g_TextSurface.height) - 1)
                    {
                        unsigned char *below = pixel + (g_TextSurface.pitch / 4) * 4;
                        if (below[3])
                        {
                            red += below[2]; green += below[1]; blue += below[0];
                            ++count;
                        }
                    }
                    if (count > 1)
                    {
                        red /= count; green /= count; blue /= count;
                    }
                    pixel[2] = static_cast<unsigned char>(red);
                    pixel[1] = static_cast<unsigned char>(green);
                    pixel[0] = static_cast<unsigned char>(blue);
                }
                pixel += 4;
            }
        }
    }
    else if (g_TextSurface.format == D3DFMT_A4R4G4B4)
    {
        unsigned short *pixel = static_cast<unsigned short *>(g_TextSurface.pixels);
        for (unsigned int row = 0; row < rows; ++row)
        {
            for (unsigned int column = 0; column < static_cast<unsigned int>(g_TextSurface.width); ++column)
            {
                if (!(*pixel & 0xf000))
                {
                    unsigned int red = 0, green = 0, blue = 0, count = 0;
                    if (column && (pixel[-1] & 0xf000))
                    {
                        red = (pixel[-1] >> 8) & 15;
                        green = (pixel[-1] >> 4) & 15;
                        blue = pixel[-1] & 15;
                        count = 1;
                    }
                    if (column < static_cast<unsigned int>(g_TextSurface.width) - 1 && (pixel[1] & 0xf000))
                    {
                        red += (pixel[1] >> 8) & 15;
                        green += (pixel[1] >> 4) & 15;
                        blue += pixel[1] & 15;
                        ++count;
                    }
                    if (row)
                    {
                        unsigned short above = pixel[-(g_TextSurface.pitch / 2)];
                        if (above & 0xf000)
                        {
                            red += (above >> 8) & 15;
                            green += (above >> 4) & 15;
                            blue += above & 15;
                            ++count;
                        }
                    }
                    if (row < static_cast<unsigned int>(g_TextSurface.height) - 1)
                    {
                        unsigned short below = pixel[g_TextSurface.pitch / 2];
                        if (below & 0xf000)
                        {
                            red += (below >> 8) & 15;
                            green += (below >> 4) & 15;
                            blue += below & 15;
                            ++count;
                        }
                    }
                    if (count > 1)
                    {
                        red /= count; green /= count; blue /= count;
                    }
                    *pixel = ((*pixel & 0xf000) | (((red >> 1) & 15) << 8)
                        | (((green >> 1) & 15) << 4) | ((blue >> 1) & 15));
                }
                ++pixel;
            }
        }
    }
    return true;
}
