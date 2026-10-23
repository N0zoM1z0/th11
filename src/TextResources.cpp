#include "TextSurface.hpp"
#include "MainRuntime.hpp"

static __forceinline unsigned short NextTextRandom16()
{
    if (g_MainRuntime.flags & 0x8000)
    {
        EnterCriticalSection(&g_MainRuntime.criticalSections[10]);
        ++g_MainRuntime.criticalDepth[10];
    }
    ++g_TextRandomState.calls;
    unsigned short value = (g_TextRandomState.seed ^ 0x9630) - 0x6553;
    g_TextRandomState.seed = (value >> 14) + (value << 2);
    if (g_MainRuntime.flags & 0x8000)
    {
        LeaveCriticalSection(&g_MainRuntime.criticalSections[10]);
        --g_MainRuntime.criticalDepth[10];
    }
    return g_TextRandomState.seed;
}

void __cdecl InitializeTextResources()
{
    if (!CreateTextSurface(D3DFMT_A4R4G4B4))
        CreateTextSurface(D3DFMT_A8R8G8B8);
    for (unsigned int i = 0; i < 256; ++i)
        g_TextSurface.noise[i] = static_cast<unsigned char>(NextTextRandom16() >> 9);
    g_LargeGothicFont = CreateFontA(32, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
        FIXED_PITCH | FF_ROMAN, g_FontFaceGothic);
    g_LargeMinchoFont = CreateFontA(32, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE,
        SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
        FIXED_PITCH | FF_ROMAN, g_FontFaceMincho);
    g_SmallGothicFont = CreateFontA(15, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
        FIXED_PITCH | FF_ROMAN, g_FontFaceGothic);
    g_SmallMinchoFont = CreateFontA(15, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
        FIXED_PITCH | FF_ROMAN, g_FontFaceMincho);
}
