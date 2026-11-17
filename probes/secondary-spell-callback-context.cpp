#include "SecondarySpellStageOwned.hpp"

// Exercise real registration and callbacks under one live, dynamic /GL caller.
// This diagnostic is not a replacement for the canonical 256-byte registrar.
extern "C" int __stdcall SecondarySpellCallbackProbe(
    SecondarySpellStageOwned *stage, int operation)
{
    if (operation == 0)
        return RegisterSecondarySpellStageChains(stage);
    if (operation == 1)
        return SecondarySpellUpdateCallback(stage);
    return SecondarySpellDrawCallback(stage);
}
