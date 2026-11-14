#include "AnimationPixelHelpers.hpp"
#include "MainRuntime.hpp"

unsigned int __stdcall ResolveAnimationTextureFormat(unsigned int index)
{
    if (*reinterpret_cast<const unsigned char *>(
        reinterpret_cast<const unsigned char *>(&g_MainRuntime) + 0x200) & 1)
    {
        D3DFORMAT format = g_AnimationTextureFormats[index];
        if (format == D3DFMT_A8R8G8B8 || format == D3DFMT_UNKNOWN)
            index = 5;
        else if (format == D3DFMT_R8G8B8)
            index = 3;
    }
    return index;
}
