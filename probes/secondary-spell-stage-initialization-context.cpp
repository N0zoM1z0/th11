#include "SecondarySpellStage.hpp"

// Dynamic caller: no spell name, index or duration is specialized away.
// This is a scoped VC9 /GL producer diagnostic, not a whole-game build.
extern "C" void __stdcall SecondarySpellStageInitProbe(
    const char *spellName, int spellIndex, int duration)
{
    InitializeSecondarySpellStage(spellName, spellIndex, duration);
}
