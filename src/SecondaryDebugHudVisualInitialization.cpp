#include "SecondaryDebugHudLifetime.hpp"
#include "AnimationDocument.hpp"
#include "ResourceFile.hpp"
#include "ErrorLog.hpp"
#include <string.h>
#include <intrin.h>

// Original game-owned data: one filename table at 0x4A8EC8, the two
// selection indices at 0x4A5710/14, and the shared global filename
// buffer at 0x4C9928. These are external references, not duplicate
// allocations or reconstructed binary address constants.
extern const char **g_SecondaryHudFilenameTable;
extern int g_SecondaryHudCharacterIndex, g_SecondaryHudPartnerIndex;
extern int g_SecondaryHudDisplayValue;         // Native 0x4A56E4
extern char g_SecondaryProgramFilename[];      // Native 0x4C9928
extern void *g_SecondaryHudPreparedData;       // Native 0x4C3238
extern const char g_SecondaryHudResourceErrorMessage[];
extern const char g_SecondaryHudLoadSkipFormat[];
void __cdecl InputAcquireHook(const char *format, ...);

// Full native 0x41A520..0x41A66F (336 bytes). The ordinary C++ call
// retains the original selection and animation file ownership. All
// compiler-private register choices belong to the VC9 /GL producer.
int __stdcall InitializeSecondaryDebugHudVisuals(SecondaryDebugHudOwned *self)
{
    const char *animationFilename = g_SecondaryHudFilenameTable[12];
    AnimationDocumentView *animation =
        AcquireAnimationDocument(0x1b, animationFilename);
    self->retainedAnimationFile =
        reinterpret_cast<AnimationFileView *>(animation);
    if (!animation)
    {
        AppendLog(&g_RuntimeLog.buffer, g_SecondaryHudResourceErrorMessage);
        return -1;
    }

    if (g_SecondaryHudPreparedData)
    {
        self->preparedData = g_SecondaryHudPreparedData;
        _ReadWriteBarrier();
        const int selection =
            (g_SecondaryHudCharacterIndex + 2) * 3 + g_SecondaryHudPartnerIndex;
        const char *selectedFilename = g_SecondaryHudFilenameTable[selection];
        InputAcquireHook(g_SecondaryHudLoadSkipFormat, selectedFilename);
        g_SecondaryHudPreparedData = 0;
    }
    else
    {
        const int selection =
            (g_SecondaryHudCharacterIndex + 2) * 3 + g_SecondaryHudPartnerIndex;
        const char *selectedFilename = g_SecondaryHudFilenameTable[selection];
        // The native clears one byte and appends an unbounded filename,
        // relying on the already established global buffer's capacity.
        g_SecondaryProgramFilename[0] = 0;
        strcat(g_SecondaryProgramFilename, selectedFilename);
        self->preparedData =
            LoadResourceFile(g_SecondaryProgramFilename, 0, 0);
        if (!self->preparedData)
        {
            AppendLog(&g_RuntimeLog.buffer, g_SecondaryHudResourceErrorMessage);
            return -1;
        }
    }

    // The member is an actual twenty-byte StageTimerView at 0x43C8.
    // SetFrame(-1) does the original first-time scale binding, flags
    // bit0 publication, and repeated float/int reset in native order.
    StageTimerView &timer = self->hudTimer;
    if (!(timer.flags & 1u))
    {
        timer.fraction = 0.0f;
        timer.current = 0;
        timer.previous = -999999;
        timer.scale = &g_AnimationTimeScale;
        timer.flags |= 1u;
    }
    *reinterpret_cast<volatile int *>(&timer.current) = 0;
    *reinterpret_cast<volatile float *>(&timer.fraction) = 0.0f;
    *reinterpret_cast<volatile int *>(&timer.previous) = -1;
    _ReadWriteBarrier();
    self->displayedValue43e4 = g_SecondaryHudDisplayValue;
    self->value4440 = -1;
    self->value4448 = -1;
    return 0;
}
