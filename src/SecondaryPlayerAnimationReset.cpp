#include "SecondaryPlayerController.hpp"
#include "VmBindings.hpp"

// Physical eight-entry animation pair view. The original controller
// class and member names are not claimed from this function.
struct SecondaryAnimationPair
{
    unsigned int primary, secondary;
    unsigned char unknown008[0xe4 - 8];
};
typedef char SecondaryAnimationPairSize[(sizeof(SecondaryAnimationPair) == 0xe4) ? 1 : -1];

namespace
{
__forceinline BoundVmView *FindControllerBoundVm(BoundVmListsView *lists,
                                                  unsigned int id)
{
    if (!id)
        return 0;
    for (BoundVmNodeView *node = lists->first; node; node = node->next)
        if (node->vm->id == id)
            return node->vm;
    for (BoundVmNodeView *node = lists->second; node; node = node->next)
        if (node->vm->id == id)
            return node->vm;
    return 0;
}

__forceinline void SignalControllerBoundVm(BoundVmListsView *lists, unsigned int id)
{
    BoundVmView *vm = FindControllerBoundVm(lists, id);
    if (vm)
    {
        *reinterpret_cast<unsigned short *>(
            reinterpret_cast<unsigned char *>(vm) + 0x37c) = 2;
        if (!vm->marker018)
        {
            for (BoundVmNodeView *node = vm->related; node; node = node->next)
            {
                *reinterpret_cast<unsigned short *>(
                    reinterpret_cast<unsigned char *>(node->vm) + 0x37c) = 2;
            }
        }
    }
}
}

// Complete original TH11 owner 0042F580..0042F69D, native private EBX
// receiver. Revoke queued animations for all eight primary/secondary slots.
void __stdcall ResetSecondaryPlayerAnimationPairs(SecondaryPlayerControllerView *self)
{
    self->flags &= ~8u;
    BoundVmListsView *lists = reinterpret_cast<BoundVmListsView *>(g_RenderBatch);
    unsigned int *ids = reinterpret_cast<unsigned int *>(
        reinterpret_cast<unsigned char *>(self) + 0x7624);
    for (int remaining = 8; remaining; --remaining)
    {
        SignalControllerBoundVm(lists, ids[-1]);
        SignalControllerBoundVm(lists, ids[0]);
        ids += 0xe4 / sizeof(*ids);
    }
    self->unknown8bc8 = 0;
}

// Global controller object pointer observed at 004A8EB4. The named
// declaration does not assert its original translation-unit ownership.
extern SecondaryPlayerControllerView *g_SecondaryPlayerControllerInstance;
extern int g_SecondaryLifeFragments;
extern SecondaryActorHudView *g_SecondaryActorHud;
extern int g_SecondaryPlayerLives;

typedef char SecondaryPlayerPatternTimeOffset[(offsetof(SecondaryPlayerControllerView, patternTime) == 0x930) ? 1 : -1];
typedef char SecondaryPlayerAuxTimeOffset[(offsetof(SecondaryPlayerControllerView, auxiliaryTime) == 0x958) ? 1 : -1];

void __stdcall ReleaseSecondaryAnimation(AnimationVmId id);
void __stdcall UpdateSecondaryLivesDisplay(SecondaryActorHudView *hud, int lives, int fragments);
void __stdcall ResetSecondaryPlayerEmitters(SecondaryDamageReferencesView *self);

// Full native 0042FEC0..0042FFEF: return controller to its active mode,
// reset timers, clear animation pairs, and refresh emitter/life rendering.
void __cdecl ResetSecondaryPlayerControllerState()
{
    SecondaryPlayerControllerView *owner = g_SecondaryPlayerControllerInstance;
    owner->mode = 1;

    owner->patternTime.SetFrame(-2);
    owner->patternTime.current = -1;
    owner->patternTime.fraction = -1.0f;

    owner->stateTime.SetFrame(-1);
    owner->auxiliaryTime.SetFrame(-1);

    owner->flags &= ~7u;
    owner->fireCount = 0;
    owner->unknown924 = 0;
    unsigned int *animationId = reinterpret_cast<unsigned int *>(
        reinterpret_cast<unsigned char *>(owner) + 0x756c);
    ReleaseSecondaryAnimation(AnimationVmId(*animationId));
    *animationId = 0;

    UpdateSecondaryLivesDisplay(g_SecondaryActorHud,
                                g_SecondaryPlayerLives, g_SecondaryLifeFragments);
    ResetSecondaryPlayerAnimationPairs(owner);
    ResetSecondaryPlayerEmitters(reinterpret_cast<SecondaryDamageReferencesView *>(owner));
}
