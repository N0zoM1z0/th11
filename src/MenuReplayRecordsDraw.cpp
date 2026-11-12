#include "MenuController.hpp"
#include "AsciiLifecycle.hpp"
#include "AsciiFormatting.hpp"
#include "SecondaryActorRuntime.hpp"
#include <time.h>

extern const char *g_MenuPracticeStageNames[];

namespace
{
template<typename T> __forceinline T &RecordAt(const void *root, unsigned int offset)
{
    return *reinterpret_cast<T *>(reinterpret_cast<unsigned char *>(const_cast<void *>(root)) + offset);
}
}

// Complete Japanese 1.00a 0x43F410..0x43F6AD, the ten-entry replay record
// display and summary. Under the original /GL menu dispatcher the receiver
// arrives in EDI rather than via an ordinary external stack ABI.
int __stdcall DrawMenuPage0A(MenuControllerView *self)
{
    if (self->page.state != 2)
        return 1;

    AsciiManagerObject *ascii = g_AsciiManager;
    AsciiManagerView *text = reinterpret_cast<AsciiManagerView *>(ascii);
    float position[3] = {48.0f, 160.0f, 0.0f};
    ascii->unknown18494 = 1;
    const int selected = RecordAt<int>(self, 0xfc);
    if (!RecordAt<int>(self, 0x1d4))
    {
        unsigned int alpha = 0xff;
        for (int slot = 1; slot <= 10; ++slot)
        {
            ascii->color = ((alpha << 8 | alpha) << 8) | 0xff0000ffu;
            const int group = self->page.selection.current;
            const unsigned char *record = g_SecondaryGameplayStatistics +
                group * 0x68d4u + selected * 0x118u + (slot - 1) * 0x1cu;
            const int score = RecordAt<int>(record, 0x18);
            const int digit = static_cast<signed char>(RecordAt<char>(record, 0x1d));
            const char *name = reinterpret_cast<const char *>(record + 0x1e);
            const double percent = RecordAt<float>(record, 0x30);
            const __time64_t timestamp = RecordAt<__time64_t>(record, 0x28);
            if (timestamp)
            {
                const struct tm *local = _localtime64(reinterpret_cast<const __time64_t *>(record + 0x28));
                const int stage = static_cast<signed char>(RecordAt<char>(record, 0x1c));
                AsciiManagerPrint(text, position,
                    "%2d  %s  %9ld%d  %.4d/%.2d/%.2d %.2d:%.2d  %s  %2.1f%%",
                    slot, name, score, digit,
                    local->tm_year + 1900, local->tm_mon + 1,
                    local->tm_mday, local->tm_hour, local->tm_min,
                    g_MenuPracticeStageNames[stage], percent);
            }
            else
            {
                AsciiManagerPrint(text, position,
                    "%2d  %s  %9ld%d  ----/--/-- --:--  Stage -  ---%%",
                    slot, name, score, digit);
            }
            alpha -= 0x10;
            position[1] = static_cast<float>(position[1] + 18.0);
        }
    }

    ascii->color = 0xffffffffu;
    unsigned char *stats = g_SecondaryGameplayStatistics +
        self->page.selection.current * 0x68d4u;
    position[0] = 328.0f;
    position[1] = 378.0f;
    AsciiManagerPrint(text, position, "    %5d", RecordAt<int>(stats, 0x590));

    const int frames = RecordAt<int>(stats, 0x594);
    position[1] = 396.0f;
    AsciiManagerPrint(text, position, "%3d:%.2d:%.2d",
                      frames / 216000, (frames / 3600) % 60, (frames / 60) % 60);
    position[1] = 414.0f;
    AsciiManagerPrint(text, position, "    %5d", RecordAt<int>(stats, 0x598 + selected * 4));

    ascii->color = 0xffffffffu;
    ascii->unknown18494 = 0;
    return 1;
}
