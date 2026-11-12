#include "SecondaryEmitterAnimation.hpp"
#include "MainRuntime.hpp"
#include "RenderBatch.hpp"

namespace
{
struct FrontDrawVmRegistryView
{
    unsigned char unknown000[0x7b562c];
    AnimationVmNode *first, *firstTail, *second, *secondTail;
    unsigned char unknown7b563c[0x7bd888 - 0x7b563c];
    unsigned int nextId;
};
}

// 00455860..004558D5: prepend the VM to the SECONDARY renderer chain.
AnimationVmId __stdcall PrependSecondaryDrawAnimationVm(AnimationVmView *vm)
{
    FrontDrawVmRegistryView *registry = reinterpret_cast<FrontDrawVmRegistryView *>(g_RenderBatch);
    AnimationVmNode *node = &vm->updateNode;
    node->vm = vm;
    node->next = 0;
    node->previous = 0;
    if (!registry->second) registry->secondTail = node;
    if (registry->second)
    {
        AnimationVmNode *first = registry->second;
        if (node->next)
        {
            first->next = node->next;
            node->next->previous = first;
        }
        node->next = first;
        first->previous = node;
    }
    registry->second = node;
    ++registry->nextId;
    if (!registry->nextId) ++registry->nextId;
    vm->unknown000 = registry->nextId;
    return AnimationVmId(registry->nextId);
}

// 00455FB0..00456018: front-chain VM without a layer override.
AnimationVmId __stdcall CreateDefaultFrontSecondaryDrawAnimationVmId(
    AnimationFileView *file, int script)
{
    if (g_MainRuntime.flags & 0x8000)
    {
        EnterCriticalSection(&g_MainRuntime.criticalSections[9]);
        ++g_MainRuntime.criticalDepth[9];
    }
    AnimationVmView *vm = AllocateSceneAnimationVm();
    vm->flags.value |= 0x40000000;
    InitializeSceneAnimationVm(vm, file, script);
    AnimationVmId id = PrependSecondaryDrawAnimationVm(vm);
    if (g_MainRuntime.flags & 0x8000)
    {
        LeaveCriticalSection(&g_MainRuntime.criticalSections[9]);
        --g_MainRuntime.criticalDepth[9];
    }
    return id;
}

// 00456020..0045608F: front-chain VM with caller-selected layer.
AnimationVmId __stdcall CreateFrontSecondaryDrawAnimationVmId(
    AnimationFileView *file, int script, int layer)
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
    AnimationVmId id = PrependSecondaryDrawAnimationVm(vm);
    if (g_MainRuntime.flags & 0x8000)
    {
        LeaveCriticalSection(&g_MainRuntime.criticalSections[9]);
        --g_MainRuntime.criticalDepth[9];
    }
    return id;
}
