#include "SecondarySpellStageOwned.hpp"
#include "SecondaryActorRuntime.hpp"
#include "MainRuntime.hpp"
#include <new>
#include <string.h>

// No second global owner: 0x4A8D6C is the same spell singleton used by
// rendering, update, time settlement and score recording.
namespace
{
__forceinline void UnregisterSpellStageChain(ChainElement *element)
{
    ChainManagerView *chain = g_ChainManager;
    if (!element)
        return;

    if (g_MainRuntime.flags & 0x8000u)
    {
        EnterCriticalSection(&g_MainRuntime.criticalSections[0]);
        ++g_MainRuntime.criticalDepth[0];
    }
    UnregisterChainElement(chain, element);
    // This is a fresh flag test in the native destructor. A callback may
    // change the synchronization flag inside UnregisterChainElement.
    if (g_MainRuntime.flags & 0x8000u)
    {
        LeaveCriticalSection(&g_MainRuntime.criticalSections[0]);
        --g_MainRuntime.criticalDepth[0];
    }
}
}

// The two StageEmbeddedVm members were constructed before this body;
// the original initializer then clears the *entire* 0x924-byte object.
__forceinline SecondarySpellStageOwned::SecondarySpellStageOwned()
{
    memset(this, 0, sizeof(*this));
    flags |= 2u;
    g_MeshEffectSpell = reinterpret_cast<MeshEffectSpellView *>(this);
}

// Native 0x40BE50..0x40BF8C. The compiler-generated member-array
// destructor runs after this ordinary C++ body, freeing both real VM stores.
SecondarySpellStageOwned::~SecondarySpellStageOwned()
{
    volatile unsigned int *ids = followerIds;
    ReleaseSecondaryAnimation(AnimationVmId(ids[0]));
    ids[0] = 0;
    ReleaseSecondaryAnimation(AnimationVmId(ids[1]));
    ids[1] = 0;
    ReleaseSecondaryAnimation(AnimationVmId(ids[2]));
    ids[2] = 0;

    UnregisterSpellStageChain(updateChain);
    UnregisterSpellStageChain(drawChain);

    g_MeshEffectSpell = 0;
}

// Native 0x40BF90..0x40C05D. Do not replace the native two VM-member
// constructors with a POD allocation or hand-coded zeroed byte buffer.
SecondarySpellStageOwned *CreateSecondarySpellStage()
{
    SecondarySpellStageOwned *stage = new SecondarySpellStageOwned;
    if (RegisterSecondarySpellStageChains(stage))
    {
        delete stage;
        return 0;
    }
    return stage;
}

// Two separate, native source owners adjacent to the lifetime functions.
// The first preserves the historical return-value pointer after deletion.
SecondarySpellStageOwned *DeleteSecondarySpellStageReturningPointer(
    SecondarySpellStageOwned *stage)
{
    delete stage;
    return stage;
}

void DestroySecondarySpellStage()
{
    SecondarySpellStageOwned *stage =
        reinterpret_cast<SecondarySpellStageOwned *>(g_MeshEffectSpell);
    if (stage)
        delete stage;
}
