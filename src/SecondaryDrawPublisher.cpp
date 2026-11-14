#include "SecondaryEmitterAnimation.hpp"
#include "AnimationVmFactory.hpp"
#include "RenderBatch.hpp"
#include "MainRuntime.hpp"

namespace
{
struct EmitterVmRegistryView
{
    unsigned char unknown000[0x7b562c];
    AnimationVmNode *first, *tail, *second, *secondTail;
    unsigned char unknown7b563c[0x7bd888 - 0x7b563c];
    unsigned int nextId;
};
}

// 004557E0. Append to the second (draw) VM list and assign the next nonzero id.
AnimationVmId __stdcall PublishSecondaryDrawAnimationVm(AnimationVmView *vm)
{
    EmitterVmRegistryView *registry = reinterpret_cast<EmitterVmRegistryView *>(g_RenderBatch);
    AnimationVmNode *node = &vm->updateNode;
    node->vm = vm;
    node->next = 0;
    node->previous = 0;
    if (!registry->second)
        registry->second = node;
    else
    {
        AnimationVmNode *tail = registry->secondTail;
        if (tail->next)
        {
            node->next = tail->next;
            tail->next->previous = node;
        }
        tail->next = node;
        node->previous = tail;
    }
    registry->secondTail = node;
    ++registry->nextId;
    if (!registry->nextId)
        ++registry->nextId;
    vm->unknown000 = registry->nextId;
    return AnimationVmId(registry->nextId);
}

// 00455DF0. Create a draw-list VM on a layer under the VM lock.
AnimationVmId __stdcall CreateSecondaryDrawAnimationVmId(AnimationFileView *file, int script, int layer)
{
    if (g_MainRuntime.flags & 0x8000)
    {
        EnterCriticalSection(&g_MainRuntime.criticalSections[9]);
        ++g_MainRuntime.criticalDepth[9];
    }
    AnimationVmView *vm = AllocateSceneAnimationVm();
    vm->flags.value |= 0x40000000;
    vm->persistent020 = layer;
    InitializeSceneAnimationVm(vm, file, script);
    AnimationVmId id = PublishSecondaryDrawAnimationVm(vm);
    if (g_MainRuntime.flags & 0x8000)
    {
        LeaveCriticalSection(&g_MainRuntime.criticalSections[9]);
        --g_MainRuntime.criticalDepth[9];
    }
    return id;
}
