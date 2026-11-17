#include "SecondarySpellStage.hpp"
#include "SecondaryActorRuntime.hpp"
#include "FrameClock.hpp"
#include <math.h>

// Native globals: the attempt records are loaded through the same table
// initialized at 0x40C650. The stage runtime selector at 0x4A8E88
// controls whether to write a new code or read a replayed code.
struct StageSpellAttemptContext
{
    unsigned char unknown000[0x74];
    int mode;
};
extern StageSpellAttemptContext *g_StageSpellAttemptContext; // 0x004A8E88
struct SpellRecordStatusView;
extern SpellRecordStatusView *g_StageSpellRecordStatus;     // 0x004A8EB8
extern int g_StageSpellDifficulty;                        // 0x004A5728

namespace
{
__forceinline unsigned char *RecordTableEntry(int index)
{
    unsigned char *data = reinterpret_cast<unsigned char *>(g_StageSpellRecordStatus);
    return *reinterpret_cast<unsigned char **>(data + index * 4);
}

__forceinline int CompletionCode(int seconds, int hundredths)
{
    // Native x86 arithmetic wraps in 32 bits before the final signed
    // remainder. Do not reduce this to a supposedly equivalent decimal
    // encoding with different overflow behavior.
    unsigned int value = (static_cast<unsigned int>(hundredths) + static_cast<unsigned int>(seconds)) * 1000u;
    value += static_cast<unsigned int>(seconds) + 0x5632u;
    value *= 100u;
    return static_cast<int>(value +
        static_cast<unsigned int>((hundredths + 33) % 100));
}
}

// Full original TH11 0x40C310..0x40C47F. This records spell timing
// through the game's real high-resolution frame clock and the native
// signed-decimal checksum, without patching or simulating machine code.
void __stdcall SettleSecondarySpellStageTime()
{
    SecondarySpellStageView *stage =
        reinterpret_cast<SecondarySpellStageView *>(g_MeshEffectSpell);
    const unsigned int flags = stage->flags;
    if (flags & 1u)
    {
        if (!(flags & 0x40u))
        {
            stage->clockStarted = GetFrameClockSeconds();
            stage->flags |= 0x40u;
        }
        return;
    }
    if (!(flags & 0x40u))
        return;

    stage->frameCountAtFinish = stage->activeFrames;
    const double elapsed = GetFrameClockSeconds() - stage->clockStarted;

    // The original fmods by 0.0167 and rounds a half-tick at 0.00835.
    const double residue = fmod(elapsed, 0.0167);
    double rounded = elapsed - residue;
    if (residue >= 0.00835)
        rounded += 0.0167;
    stage->roundedElapsed = rounded;

    const double whole = floor(stage->roundedElapsed);
    int seconds = static_cast<int>(whole);
    if (seconds >= 1000)
        seconds = 999;
    const int hundredths = static_cast<int>(
        (stage->roundedElapsed - whole) * 100.0);

    stage->roundedElapsed = 0.0;
    stage->flags &= ~0x40u;
    stage->recordCode = CompletionCode(seconds, hundredths);

    const int slot = stage->recordIndex;
    if (g_StageSpellAttemptContext->mode == 0)
    {
        unsigned char *record = RecordTableEntry(0x7 + g_StageSpellDifficulty);
        *reinterpret_cast<int *>(record + 0x3c + slot * 4) =
            stage->recordCode;
    }
    else
    {
        unsigned char *record = RecordTableEntry(
            g_StageSpellDifficulty * 9 + 0x2d);
        stage->recordCode =
            *reinterpret_cast<int *>(record + 0x3c + slot * 4);
        if (CheckSecondarySpellRecordCode(stage))
            stage->recordCode = 0x06ae9c24;
    }
    ++stage->recordIndex;
}
