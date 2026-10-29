// Placement construction preserves real member/EH lifetimes; never executed.
#include "AnimationRendererLifecycle.hpp"
#include <new>
extern "C" void *__stdcall AnimationRendererLifecycleProbe(int kind, AnimationRenderer *self, AnimationVmView *vm)
{
    switch (kind)
    {
    case 0: return new (self) AnimationRenderer;
    case 1: self->~AnimationRenderer(); break;
    case 2: InitializeAnimationWorldBuffer(); break;
    case 3: DestroyRendererAnimationVm(self, vm); break;
    }
    return 0;
}
