#include "AnimationVmFactory.hpp"
#include "RenderBatch.hpp"
#include "PerspectiveCamera.hpp"
#include "MainRuntime.hpp"
#include <string.h>

struct SceneVmRegistryView
{
    unsigned char unknown000[0x7b562c];
    AnimationVmNode *first, *tail, *second;
    unsigned char unknown7b5638[0x7bd888 - 0x7b5638];
    unsigned int nextId;

    // Observed member-call boundary at 0x405D60; implementation remains external.
    int Execute(AnimationVmView *vm);
};

static __forceinline CameraVector SceneZeroVector()
{
    CameraVector result = {0.0f, 0.0f, 0.0f};
    return result;
}

AnimationVmId __stdcall PublishSceneAnimationVm(AnimationVmView *vm)
{
    SceneVmRegistryView *registry = reinterpret_cast<SceneVmRegistryView *>(g_RenderBatch);
    AnimationVmNode *node = &vm->updateNode;
    node->vm = vm;
    node->next = 0;
    node->previous = 0;
    if (!registry->first)
        registry->first = node;
    else
    {
        AnimationVmNode *tail = registry->tail;
        if (tail->next)
        {
            node->next = tail->next;
            tail->next->previous = node;
        }
        tail->next = node;
        node->previous = tail;
    }
    registry->tail = node;
    ++registry->nextId;
    if (!registry->nextId)
        ++registry->nextId;
    vm->unknown000 = registry->nextId;
    return AnimationVmId(registry->nextId);
}

void __stdcall InitializeSceneAnimationVm(AnimationVmView *vm, AnimationFileView *file, int script)
{
    vm->flags.value |= 0x40000000;
    vm->scriptId = static_cast<unsigned short>(script);
    *reinterpret_cast<CameraVector *>(&vm->persistent3e8) = SceneZeroVector();
    *reinterpret_cast<CameraVector *>(reinterpret_cast<unsigned char *>(vm) + 0x3dc) = SceneZeroVector();
    *reinterpret_cast<CameraVector *>(reinterpret_cast<unsigned char *>(vm) + 0x3f4) = SceneZeroVector();
    reinterpret_cast<unsigned char *>(vm)[0x41d] = 0x10;
    reinterpret_cast<unsigned char *>(vm)[0x41c] = 0x10;
    BindAnimationFileScript(file, script, vm);
}

void __stdcall BindAnimationFileScript(AnimationFileView *file, int script, AnimationVmView *vm)
{
    if (file->scripts[script] && !file->bindingDisabled)
    {
        vm->scriptId = static_cast<unsigned short>(script);
        vm->fileId = file->id;
        vm->flags.value &= ~0x600u;
        vm->file = file;
        vm->scriptStart = file->scripts[script];
        vm->instruction = vm->scriptStart;
        vm->time.SetFrame(-1);
        vm->flags.value &= ~1u;
        reinterpret_cast<SceneVmRegistryView *>(g_RenderBatch)->Execute(vm);
        ++*reinterpret_cast<unsigned int *>(g_RenderBatch->unknown000 + 0xa0);
        return;
    }
    memset(vm, 0, sizeof(*vm));
}

AnimationVmId __stdcall CreateAnimationVmId(AnimationFileView *file, int script, int layer)
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
    AnimationVmId id = PublishSceneAnimationVm(vm);
    if (g_MainRuntime.flags & 0x8000)
    {
        LeaveCriticalSection(&g_MainRuntime.criticalSections[9]);
        --g_MainRuntime.criticalDepth[9];
    }
    return id;
}
