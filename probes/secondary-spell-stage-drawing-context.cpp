#include "SecondarySpellStage.hpp"

// Keep the 370-byte draw owner live without specializing flags or VM IDs.
// This source is linked only for the local /GL codegen comparison.
extern "C" int __stdcall SecondarySpellStageDrawProbe(SecondarySpellStageView *stage)
{
    return DrawSecondarySpellStage(stage);
}
