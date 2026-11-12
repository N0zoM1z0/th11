#include "SecondaryDebugOverlay.hpp"
#include "SecondaryActorState.hpp"
#include "AsciiFormatting.hpp"
#include "AsciiLifecycle.hpp"
#include "AnimationVmDraw.hpp"
#include "RenderBatch.hpp"

namespace
{
__forceinline unsigned int &AsciiTextColor()
{
    return *reinterpret_cast<unsigned int *>(
        reinterpret_cast<unsigned char *>(g_AsciiManager) + 0x18480);
}

// Native ECL debug's two sprite draw endpoints and three coordinate cells
// are physical offsets, not yet evidence of an original nominal C++ type.
__forceinline void DrawDebugSelection(SecondaryEclDebugView *session)
{
    float *position = reinterpret_cast<float *>(
        reinterpret_cast<unsigned char *>(session) + 0x6a4);
    position[0] = 320.0f;
    position[1] = 240.0f;
    position[2] = 0.0f;
    DrawUnrotatedAnimationVm(g_RenderBatch,
        reinterpret_cast<AnimationVmView *>(
            reinterpret_cast<unsigned char *>(session) + 0x2c8));
}

__forceinline AsciiManagerView *AsciiOwner()
{
    return reinterpret_cast<AsciiManagerView *>(g_AsciiManager);
}

__forceinline void PrintLabel(const float *p, const char *format)
{
    AsciiManagerPrintSmall(AsciiOwner(), p, format);
}

__forceinline unsigned int ReadWordAt(const void *object, int offset)
{
    return *reinterpret_cast<const unsigned int *>(
        reinterpret_cast<const unsigned char *>(object) + offset);
}
}

// 0x40DAC0..0x40DDB8 (761 bytes of code, plus a separately identified
// four-entry switch table at 0x40DDBC). Four distinct debug display modes
// print file/ECL, loading, position or enemy information. The original
// accepts a private ESI session; the ordinary source signature is only an
// ABI hypothesis until the real caller's /GL ownership is established.
int __fastcall RenderSecondaryDebugOverlay(SecondaryEclDebugView *session)
{
    float position[3] = {0.0f, 0.0f, 0.0f};
    PrintLabel(position, "SprtView\n");

    switch (session->state)
    {
    case 1:
        position[0] = 42.0f;
        position[1] = 16.0f;
        if (session->fileCount)
            AsciiManagerPrintSmall(AsciiOwner(), position, "File %s",
                session->filenames[session->selections[1].current]);
        else
            PrintLabel(position, "File not found.");

        position[0] = 26.0f;
        if (g_SecondaryMaster)
        {
            void *namesOwner = *reinterpret_cast<void **>(
                reinterpret_cast<unsigned char *>(g_SecondaryMaster) + 0x64);
            unsigned char *names = *reinterpret_cast<unsigned char **>(
                reinterpret_cast<unsigned char *>(namesOwner) + 0x8c);
            const char *name = *reinterpret_cast<const char **>(
                names + session->selections[2].current * 8);
            AsciiManagerPrintSmall(AsciiOwner(), position, "Ecl %s", name);
        }
        else
            AsciiManagerPrintSmall(AsciiOwner(), position, "Ecl %d",
                session->selections[2].current);

        position[0] = 36.0f;
        PrintLabel(position, "Quit");
        PrintLabel(position, ">");
        AsciiTextColor() = 0xffffffffu;
        if (session->flags & 2u) DrawDebugSelection(session);
        return 1;

    case 2:
        position[0] = 32.0f;
        position[1] = 16.0f;
        AsciiTextColor() = 0xffff4040u;
        AsciiManagerPrintSmall(AsciiOwner(), position, "Loading %s",
            session->filenames[session->selections[1].current]);
        AsciiTextColor() = 0xffffffffu;
        return 1;

    case 3:
        position[0] = 32.0f;
        position[1] = 16.0f;
        AsciiTextColor() = 0xffa0a0a0u;
        AsciiManagerPrintSmall(AsciiOwner(), position, "Pos %.3d %.3d",
            static_cast<int>(session->value700),
            static_cast<int>(session->value6fc));
        AsciiTextColor() = 0xffffffffu;
        if (session->flags & 2u) DrawDebugSelection(session);
        return 1;

    case 4:
        position[0] = 500.0f;
        position[1] = 240.0f;
        AsciiManagerPrintSmall(AsciiOwner(), position, "Enemy %d",
            ReadWordAt(g_SecondaryMaster, 0x70));
        return 1;
    }
    return 1;
}

// Native 0x40DDE0..0x40DDE9: the registered owner preserves the session in
// ESI while calling the larger selector. The callee's private ABI and exact
// ten-byte callback shape are separate compile/link questions.
int __fastcall RenderSecondaryDebugOverlayCallback(SecondaryEclDebugView *session)
{
    return RenderSecondaryDebugOverlay(session);
}
