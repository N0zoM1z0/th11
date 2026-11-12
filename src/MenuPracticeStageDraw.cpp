#include "MenuPracticeStage.hpp"
#include "AsciiLifecycle.hpp"
#include "AsciiFormatting.hpp"
#include "SecondaryActorRuntime.hpp"

// Original read-only six-stage label table at 0x4A3AC8.
extern const char *g_MenuPracticeStageNames[];

// Full 0x43DAA0..0x43DC4A (427 bytes) of the Japanese v1.00a practice-stage
// draw owner. Its original private entry receives the menu in EDI from the
// caller; the source expresses that receiver conventionally.
int __stdcall DrawMenuPage08(MenuControllerView *self)
{
    if (static_cast<unsigned int>(self->page.state - 2) > 1u)
        return 1;

    AsciiManagerObject *ascii = g_AsciiManager;
    AsciiManagerView *text = reinterpret_cast<AsciiManagerView *>(ascii);
    float position[3] = {80.0f, 80.0f, 0.0f};
    ascii->unknown18494 = 1;
    if (self->page.time.current >= 10 || self->page.state == 3)
    {
        if (g_MeshEffectSelection0 == 0)
        {
            position[0] = 168.0f;
            position[1] = 216.0f;
        }
        else
        {
            position[0] = 296.0f;
            position[1] = 216.0f;
        }
        for (int stage = 1; stage < 7; ++stage)
        {
            unsigned int group = g_MeshEffectSelection1 + 3u * g_MeshEffectSelection0;
            unsigned int item = stage + 6u * g_SecondaryActorDifficulty;
            const unsigned char *record = g_SecondaryGameplayStatistics +
                group * 0x68d4u + item * 8u;
            if (self->page.selection.current == stage - 1)
            {
                if (!record[0x5a9])
                    ascii->color = 0xffdfdfdfu;
                else if (self->page.state == 3 && (self->page.time.current % 4) >= 2)
                    ascii->color = 0xff000000u;
                else
                    ascii->color = 0xffffff00u;
            }
            else
                ascii->color = 0xff808080u;

            // The native AsciiManagerPrint call is /GL-specialized to pass
            // the text-manager receiver in ECX and position in EBX.
            if (!record[0x5a9])
                AsciiManagerPrint(text, position, "%s  ---------",
                                  g_MenuPracticeStageNames[stage]);
            else
                AsciiManagerPrint(text, position, "%s  %.8d0",
                                  g_MenuPracticeStageNames[stage],
                                  *reinterpret_cast<const int *>(record + 0x5a4));
            position[1] = static_cast<float>(position[1] + 18.0);
        }
    }
    ascii->color = 0xffffffffu;
    ascii->unknown18494 = 0;
    return 1;
}
