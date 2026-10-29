#include "SecondaryActorRuntime.hpp"

AnimationVmView *__stdcall ClearMissingAnimationId(unsigned int *id)
{
    AnimationVmView *vm = FindAnimationVmById(g_RenderBatch, AnimationVmId(*id));
    if (!vm) *id = 0;
    return vm;
}

int __stdcall ValidateSecondaryAnimationId(unsigned int *id)
{
    AnimationVmView *vm = FindAnimationVmById(g_RenderBatch, AnimationVmId(*id));
    if (!vm) *id = 0;
    return vm != 0;
}

void __stdcall SignalSecondaryAnimation(unsigned int *id)
{
    AnimationVmView *vm = FindAnimationVmById(g_RenderBatch, AnimationVmId(*id));
    if (vm)
    {
        BoundVmView *bound = reinterpret_cast<BoundVmView *>(vm);
        reinterpret_cast<SecondaryActorAnimationView *>(vm)->event = 1;
        if (!bound->marker018)
            for (BoundVmNodeView *node = bound->related; node; node = node->next)
                reinterpret_cast<SecondaryActorAnimationView *>(node->vm)->event = 1;
    }
}

void __stdcall ReleaseSecondaryAnimation(unsigned int id)
{
    AnimationVmView *vm = FindAnimationVmById(g_RenderBatch, AnimationVmId(id));
    if (vm)
    {
        BoundVmView *bound = reinterpret_cast<BoundVmView *>(vm);
        vm->flags.value |= 0x4000000;
        if (!bound->marker018)
            for (BoundVmNodeView *node = bound->related; node; node = node->next)
                node->vm->flags |= 0x4000000;
    }
}

namespace
{
__forceinline void PositionVm(AnimationVmView *vm, const CameraVector *position)
{
    BoundVmView *bound = reinterpret_cast<BoundVmView *>(vm);
    CameraVector &destination = reinterpret_cast<SecondaryActorAnimationView *>(vm)->position;
    destination.x = static_cast<float>(position->x + 32.0 + 192.0);
    destination.y = static_cast<float>(position->y + 16.0);
    destination.z = position->z;
    if (!bound->marker018)
        for (BoundVmNodeView *node = bound->related; node; node = node->next)
        {
            CameraVector &child = reinterpret_cast<SecondaryActorAnimationView *>(node->vm)->position;
            child.x = static_cast<float>(position->x + 32.0 + 192.0);
            child.y = static_cast<float>(position->y + 16.0);
            child.z = position->z;
        }
}
}

void __stdcall PositionSecondaryAnimationVm(AnimationVmView *vm, const CameraVector *position)
{ PositionVm(vm, position); }

void __stdcall PositionSecondaryAnimation(unsigned int id, const CameraVector *position)
{
    AnimationVmView *vm = FindAnimationVmById(g_RenderBatch, AnimationVmId(id));
    if (vm) PositionVm(vm, position);
}

void __stdcall PositionSecondaryAnimationRaw(unsigned int id, const CameraVector *position)
{
    AnimationVmView *vm = FindAnimationVmById(g_RenderBatch, AnimationVmId(id));
    if (vm)
    {
        BoundVmView *bound = reinterpret_cast<BoundVmView *>(vm);
        reinterpret_cast<SecondaryActorAnimationView *>(vm)->position = *position;
        if (!bound->marker018)
            for (BoundVmNodeView *node = bound->related; node; node = node->next)
                reinterpret_cast<SecondaryActorAnimationView *>(node->vm)->position = *position;
    }
}

int __stdcall SecondaryGlobalStatusBits()
{
    unsigned int flags = SecondaryActorSpell()->flags;
    if ((flags & 1) && (flags & 0x20)) return 1;
    return 0;
}
