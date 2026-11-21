#include "SecondaryHud.hpp"
#include "SecondaryActorState.hpp"
#include "AnimationImmediateDraw.hpp"
#include "AnimationImmediateDrawViews.hpp"
#ifdef ANIMATION_VM_RENDERER_OWNER_MATCH
#include "AnimationVmRendererOwner.cpp"
#endif

// Original TH11 HUD front-layer drawing at 0x41C5F0. The owner is
// supplied through the private VC9 call context by its registered
// 0x41CFC0 callback; the nine primary VMs are physical C++ members.
int __stdcall DrawSecondaryHudFront(SecondaryHudView *self)
{
    AnimationVmView *current = self->primary;
    int remaining = 9;
    do
    {
        current->DrawByMode(g_RenderBatch);
        current = reinterpret_cast<AnimationVmView *>(
            reinterpret_cast<unsigned char *>(current) + 0x434);
    } while (--remaining);

    SecondaryMasterView *master = g_SecondaryMaster;
    if (master)
    {
        SecondaryActorManagerView *actor = master->Slot(0);
        if (actor)
        {
            const unsigned int flags = *reinterpret_cast<const unsigned int *>(
                reinterpret_cast<const unsigned char *>(actor) + 0x25bc);
            if (((~(flags >> 5)) & 1u) != 0 &&
                ((~flags) & 1u) != 0)
                self->indicator.DrawByMode(g_RenderBatch);
        }
    }
    return 1;
}

// Native chain callback 0x41CFC0; unlike the update callback, its
// renderer owner is passed privately in EBX after ordinary /GL lowering.
int __fastcall SecondaryHudFrontDrawCallback(SecondaryHudView *self)
{
    return DrawSecondaryHudFront(self);
}
