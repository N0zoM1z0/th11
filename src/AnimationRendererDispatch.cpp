#include "AnimationRendererDispatch.hpp"
#include "MainRuntime.hpp"
#include <new>

namespace
{
typedef int (__fastcall *VmCallback)(AnimationVmView *);
__forceinline AnimationVmView *&DrawNext(AnimationVmView *vm)
{
    return *reinterpret_cast<AnimationVmView **>(&vm->unknown01c);
}
__forceinline VmCallback UpdateCallback(AnimationVmView *vm)
{
    return *reinterpret_cast<VmCallback *>(reinterpret_cast<unsigned char *>(vm) + 0x410);
}
__forceinline VmCallback DrawCallback(AnimationVmView *vm)
{
    return *reinterpret_cast<VmCallback *>(reinterpret_cast<unsigned char *>(vm) + 0x414);
}
__forceinline void EnterVmSection()
{
    if (g_MainRuntime.flags & 0x8000)
    {
        EnterCriticalSection(&g_MainRuntime.criticalSections[9]);
        ++g_MainRuntime.criticalDepth[9];
    }
}
__forceinline void LeaveVmSection()
{
    if (g_MainRuntime.flags & 0x8000)
    {
        LeaveCriticalSection(&g_MainRuntime.criticalSections[9]);
        --g_MainRuntime.criticalDepth[9];
    }
}
}

int __stdcall UpdateAnimationRenderer(AnimationRenderer *self)
{
    EnterVmSection();
    AnimationVmView *tails[29];
    for (int i = 0; i < 29; ++i)
    {
        tails[i] = reinterpret_cast<AnimationVmView *>(&self->additionalVms[i]);
        DrawNext(tails[i]) = 0;
    }
    AnimationVmNode *node = self->activeHead;
    while (node)
    {
        AnimationVmView *vm = node->vm;
        AnimationVmNode *next = node->next;
        if (vm->flags.value & 0x04000000)
            DestroyRendererAnimationVm(self, vm);
        else
        {
            if (UpdateCallback(vm)) UpdateCallback(vm)(vm);
            if (ExecuteAnimationVm(vm))
                DestroyRendererAnimationVm(self, vm);
            else
            {
                DrawNext(tails[vm->persistent020]) = vm;
                tails[vm->persistent020] = vm;
                DrawNext(vm) = 0;
            }
        }
        ++self->processedVmCount;
        node = next;
    }
    LeaveVmSection();
    return 1;
}

int __stdcall UpdateOtherAnimationRenderer(AnimationRenderer *self)
{
    EnterVmSection();
    AnimationVmView *tails[2];
    tails[0] = reinterpret_cast<AnimationVmView *>(&self->additionalVms[29]);
    tails[1] = reinterpret_cast<AnimationVmView *>(&self->additionalVms[30]);
    DrawNext(tails[0]) = 0;
    DrawNext(tails[1]) = 0;
    self->processedVmCount = 0;
    AnimationVmNode *node = self->otherHead;
    while (node)
    {
        AnimationVmView *vm = node->vm;
        AnimationVmNode *next = node->next;
        if (vm->flags.value & 0x04000000)
            DestroyRendererAnimationVm(self, vm);
        else
        {
            if (UpdateCallback(vm)) UpdateCallback(vm)(vm);
            if (ExecuteAnimationVm(vm))
                DestroyRendererAnimationVm(self, vm);
            else
            {
                if (vm->persistent020 != 29 && vm->persistent020 != 30)
                    vm->persistent020 = 29;
                DrawNext(tails[vm->persistent020 - 29]) = vm;
                tails[vm->persistent020 - 29] = vm;
                DrawNext(vm) = 0;
            }
        }
        ++self->processedVmCount;
        node = next;
    }
    LeaveVmSection();
    return 1;
}

int __stdcall DrawAnimationRendererLayer(AnimationRenderer *self, int layer)
{
    EnterVmSection();
    AnimationVmView *vm = DrawNext(reinterpret_cast<AnimationVmView *>(&self->additionalVms[layer]));
    while (vm)
    {
        if (!(vm->flags.value & 0x04000000))
        {
            if (DrawCallback(vm)) DrawCallback(vm)(vm);
            DrawAnimationVmByMode(reinterpret_cast<RenderBatchView *>(self), vm);
        }
        vm = DrawNext(vm);
    }
    LeaveVmSection();
    return 1;
}

AnimationVmView *__stdcall AllocateSceneAnimationVm()
{
    AnimationRenderer *self = reinterpret_cast<AnimationRenderer *>(g_RenderBatch);
    StageEmbeddedVm *vm = &self->vms[self->nextPoolSlot];
    if (self->used[self->nextPoolSlot])
    {
        self->nextPoolSlot = static_cast<int>(static_cast<unsigned int>(self->nextPoolSlot) + 1) % 4096;
        vm = &self->vms[self->nextPoolSlot];
        if (self->used[self->nextPoolSlot])
        {
            vm = new StageEmbeddedVm;
            ResetAnimationVm(reinterpret_cast<AnimationVmView *>(vm));
        }
        else self->used[self->nextPoolSlot] = 1;
    }
    else self->used[self->nextPoolSlot] = 1;
    self->nextPoolSlot = static_cast<int>(static_cast<unsigned int>(self->nextPoolSlot) + 1) % 4096;
    return reinterpret_cast<AnimationVmView *>(vm);
}
