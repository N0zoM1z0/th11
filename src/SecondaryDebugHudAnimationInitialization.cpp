#include "SecondaryDebugHudLifetime.hpp"
#include "SecondaryActorRuntime.hpp"
#include "SecondaryPlayerDeath.hpp"
#include "AsciiLifecycle.hpp"
#include "AnimationVmFactory.hpp"
#include "MainRuntime.hpp"
#include "SecondaryDialogue.hpp"
#include "VmBindings.hpp"

// Original game globals already shared by the replay and HUD owners.
// They are external game storage, not new copies owned by this TU.
extern unsigned int g_ReplayStageCharacter, g_ReplayModeFlags;
extern unsigned char g_PauseRecordShotType;

namespace
{
__forceinline AnimationVmView *EmbeddedVm(StageEmbeddedVm *member)
{
    return reinterpret_cast<AnimationVmView *>(member);
}

__forceinline AnimationFileView *HudFrontFile(SecondaryDebugHudOwned *hud)
{
    return reinterpret_cast<AnimationFileView *>(hud->ownerMarker);
}

__forceinline void PauseHudChain(ChainElement *node)
{
    if (node)
        node->flags |= 2u;
}

// The original animation allocator uses the renderer lock at
// 0x4C3B68 (MainRuntime criticalSections[9]).
__forceinline void LockHudAnimations()
{
    if (g_MainRuntime.flags & 0x8000u)
    {
        EnterCriticalSection(&g_MainRuntime.criticalSections[9]);
        ++g_MainRuntime.criticalDepth[9];
    }
}

__forceinline void UnlockHudAnimations()
{
    if (g_MainRuntime.flags & 0x8000u)
    {
        LeaveCriticalSection(&g_MainRuntime.criticalSections[9]);
        --g_MainRuntime.criticalDepth[9];
    }
}

__forceinline AnimationVmId SpawnHudAnimation(
    AnimationFileView *file, int script)
{
    LockHudAnimations();
    AnimationVmView *vm = AllocateSceneAnimationVm();
    vm->flags.value |= 0x40000000u;
    vm->persistent020 = 0x16;
    InitializeSceneAnimationVm(vm, file, script);
    AnimationVmId id = PublishSceneAnimationVm(vm);
    UnlockHudAnimations();
    return id;
}

// In the native owner, these retained scene animations publish directly
// into the renderer intrusive list. The same invariant is shared with
// PublishSceneAnimationVm, but this local path can inline at its caller.
struct HudAnimationRegistryView
{
    unsigned char unknown000[0x7b562c];
    AnimationVmNode *first, *tail, *second;
    unsigned char unknown7b5638[0x7bd888 - 0x7b5638];
    unsigned int nextId;
};
typedef char HudAnimationRegistryFirstOffset[
    (offsetof(HudAnimationRegistryView, first) == 0x7b562c) ? 1 : -1];
typedef char HudAnimationRegistryNextIdOffset[
    (offsetof(HudAnimationRegistryView, nextId) == 0x7bd888) ? 1 : -1];

__forceinline AnimationVmId PublishHudAnimationInline(AnimationVmView *vm)
{
    HudAnimationRegistryView *registry =
        reinterpret_cast<HudAnimationRegistryView *>(g_RenderBatch);
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

__forceinline AnimationVmId SpawnHudRetainedAnimation(
    AnimationFileView *file, int script)
{
    LockHudAnimations();
    AnimationVmView *vm = AllocateSceneAnimationVm();
    vm->flags.value |= 0x40000000u;
    vm->persistent020 = 0x16;
    InitializeSceneAnimationVm(vm, file, script);
    AnimationVmId id = PublishHudAnimationInline(vm);
    UnlockHudAnimations();
    return id;
}

__forceinline void SignalHudAnimation(unsigned int id, short event)
{
    if (!id)
        return;
    BoundVmView *vm = FindBoundVm(id);
    if (!vm)
        return;

    reinterpret_cast<SecondaryActorAnimationView *>(vm)->event = event;
    if (!vm->marker018)
        for (BoundVmNodeView *node = vm->related; node; node = node->next)
            reinterpret_cast<SecondaryActorAnimationView *>(node->vm)->event = event;
}

// Native mode at 0x4C37D8 is a field of the real MainRuntime object.
__forceinline int SceneMode()
{
    return *reinterpret_cast<const int *>(
        reinterpret_cast<const unsigned char *>(&g_MainRuntime) + 0x558);
}

// The original checks all four bytes beginning at 0x4A573C, rather
// than just the low byte used by score-record serialization.
__forceinline unsigned int RecordShotState()
{
    return *reinterpret_cast<const unsigned int *>(&g_PauseRecordShotType);
}
}

// Native 0x41A6D0..0x41AD22 (1,619-byte owning interval). The HUD
// rebinds its real 9/4/2/1 embedded VMs, creates scene animations on
// separate stage/mode paths, then signals the retained effect IDs.
void InitializeSecondaryDebugHudAnimations()
{
    SecondaryDebugHudOwned *hud =
        reinterpret_cast<SecondaryDebugHudOwned *>(g_SecondaryActorHud);

    PauseHudChain(hud->updateChain);
    PauseHudChain(hud->drawChain);
    PauseHudChain(hud->overlayChain);

    if (!hud->initialVmId)
    {
        AnimationVmId initialId = CreateAnimationVmId(HudFrontFile(hud), 0, 0x16);
        hud->initialVmId = initialId.value;
    }

    if (!(EmbeddedVm(&hud->mainVms[0])->flags.value & 1u))
    {
        int i = 0;
        do
        {
            BindAnimationScript(HudFrontFile(hud), i + 10, EmbeddedVm(&hud->mainVms[i]));
            ++i;
        } while (i < 9);

        unsigned int overlayIndex = 0;
        do
        {
            BindAnimationScript(HudFrontFile(hud), overlayIndex + 0x20,
                                EmbeddedVm(&hud->overlayVms[overlayIndex]));
            ++overlayIndex;
        } while (overlayIndex < 4u);

        i = 0;
        do
        {
            AnimationFileView *asciiFile = reinterpret_cast<AnimationFileView *>(g_AsciiManager->animation);
            BindAnimationScript(asciiFile, i + 3, EmbeddedVm(&hud->auxiliaryVms[i]));
            ++i;
        } while (i < 2);
    }

    UpdateSecondaryLivesDisplay(g_SecondaryActorHud,
                                g_SecondaryPlayerLives, g_SecondaryLifeFragments);

    if (SceneMode() != 8 && !(g_StageResourceFlags & 0x20u))
    {
        SpawnHudAnimation(HudFrontFile(hud), 0);
        SpawnHudAnimation(HudFrontFile(hud), 1);
    }
    if (g_StageResourceFlags & 0x20u)
        SpawnHudRetainedAnimation(HudFrontFile(hud), 0x46);

    BindAnimationScript(reinterpret_cast<AnimationFileView *>(g_AsciiManager->animation),
                        0, EmbeddedVm(&hud->persistentVm));

    if (g_StageVariant == 1 && g_SecondaryDialogueSession->mode == 0 &&
        RecordShotState() == 0)
        SpawnHudAnimation(HudFrontFile(hud), 0x2c);

    if (g_ReplayModeFlags)
    {
        const int script = static_cast<int>(g_ReplayStageCharacter) + 0x39;
        hud->effectVmId = SpawnHudRetainedAnimation(HudFrontFile(hud), script).value;
        SignalHudAnimation(hud->effectVmId, 3);
    }

    const int script = static_cast<int>(g_ReplayStageCharacter) + 0x3e;
    hud->secondaryEffectVmId = SpawnHudRetainedAnimation(HudFrontFile(hud), script).value;
    SignalHudAnimation(hud->effectVmId, 3);
    hud->reset43fc = 0;
}
