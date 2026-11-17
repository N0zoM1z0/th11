#include "SecondarySpellStageOwned.hpp"

// Keeps the real owner factory and both destruction paths reachable.
// The original gameplay chooses these paths dynamically.
extern "C" void *__stdcall SecondarySpellStageLifecycleProbe(int operation)
{
    if (operation == 0)
        return CreateSecondarySpellStage();
    if (operation == 1)
    {
        DestroySecondarySpellStage();
        return 0;
    }
    if (operation == 2)
        return DeleteSecondarySpellStageReturningPointer(
            reinterpret_cast<SecondarySpellStageOwned *>(g_StagePrimary));
    return 0;
}
