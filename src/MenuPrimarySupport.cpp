#include "MenuPrimary.hpp"
#include "AnimationVmFactory.hpp"
#include "SecondaryActorRuntime.hpp"
#include "VmBindings.hpp"

void __stdcall StartMenuAnimation(MenuControllerView *self, int script)
{
    MenuAnimationId(self, script) = CreateAnimationVmId(self->page.file, script, 0x16);
}

void __stdcall StopMenuAnimation(MenuControllerView *self, int script)
{
    AnimationVmView *vm = FindAnimationVmById(g_RenderBatch, MenuAnimationId(self, script));
    if (vm)
    {
        reinterpret_cast<SecondaryActorAnimationView *>(vm)->event = 1;
        BoundVmView *bound = reinterpret_cast<BoundVmView *>(vm);
        if (!bound->marker018)
            for (BoundVmNodeView *node = bound->related; node; node = node->next)
                reinterpret_cast<SecondaryActorAnimationView *>(node->vm)->event = 1;
    }
    MenuAnimationId(self, script) = AnimationVmId(0);
}

void __stdcall SignalLockedMenuEntry(MenuControllerView *self, int script)
{
    AnimationVmView *parent = FindAnimationVmById(g_RenderBatch, self->page.primaryVmId);
    if (!parent) self->page.primaryVmId = AnimationVmId(0);
    AnimationVmView *child = 0;
    for (AnimationVmNode *node = &parent->drawNode; node; node = node->next)
        if (static_cast<short>(node->vm->scriptId) == script)
        {
            child = node->vm;
            break;
        }
    reinterpret_cast<SecondaryActorAnimationView *>(child)->event = 0x1d;
}

int __cdecl IsMenuExtraUnlocked()
{
    unsigned char *statistics = g_SecondaryGameplayStatistics;
    return (statistics[0x2ddf0] & 0x10) || (statistics[0x2ddf1] & 0x10) ||
        (statistics[0x2ddf2] & 0x10) || (statistics[0x2ddf3] & 0x10) ||
        (statistics[0x2ddf4] & 0x10) || (statistics[0x2ddf5] & 0x10);
}
