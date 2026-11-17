#include "SecondarySpellStageOwned.hpp"
#include "SecondaryActorRuntime.hpp"

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
    {
        SecondarySpellStageOwned *stage =
            reinterpret_cast<SecondarySpellStageOwned *>(g_MeshEffectSpell);
        delete stage;
        return 0;
    }
    return 0;
}
