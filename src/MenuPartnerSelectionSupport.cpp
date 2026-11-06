#include "MenuPartnerSelection.hpp"
#include "VmBindings.hpp"

void __stdcall HideMenuAnimationTree(const AnimationVmId &id)
{
    AnimationVmView *vm = FindAnimationVmById(g_RenderBatch, id);
    if (vm)
    {
        vm->flags.value &= ~2u;
        BoundVmView *bound = reinterpret_cast<BoundVmView *>(vm);
        if (!bound->marker018)
            for (BoundVmNodeView *node = bound->related; node; node = node->next)
                node->vm->flags &= ~2u;
    }
}
