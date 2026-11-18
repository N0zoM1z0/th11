#include "SecondarySpellStage.hpp"
#include "SecondaryActorState.hpp"
#include "SecondaryActorRuntime.hpp"
#include "SecondaryPlayerController.hpp"
#include "AnimationVmFactory.hpp"
#include "FormattedTextureText.hpp"
#include "VmBindings.hpp"
#include "MeshEffects.hpp"

// Physical, independently owned globals. The literal addresses below
// describe observed data; no second copies are allocated by this TU.
struct SpellStageRuntimeView
{
    unsigned char unknown000[0x184ac];
    AnimationFileView *file;
};
struct SpellStageEventView
{
    unsigned char unknown000[0x14];
    int count;
    unsigned char unknown018[4];
    SecondaryActorManagerView *primaryActor;
    unsigned char unknown020[0x48 - 0x20];
    AnimationFileView *file;
};
struct SpellRecordStatusView
{
    unsigned char unknown000[0x10];
    int mode;
};

extern SpellStageRuntimeView *g_StageSpellRuntime;       // 0x004A8D58
extern SpellRecordStatusView *g_StageSpellRecordStatus;  // 0x004A8EB8
extern unsigned char *g_StageSpellRecordData;            // 0x004A8EBC
extern AnimationFileView *g_StageSpellOverlayFile;       // 0x004C3808
extern int g_StageSpellLevel, g_StageSpellDifficulty;    // 0x004A5720, 0x004A5728
extern int g_StageSpellProgress;                         // 0x004A5730
struct MeshEffectEventView;
extern MeshEffectEventView *g_MeshEffectEvent;
void __stdcall ResetAndBindStageAnimationVm(
    AnimationVmView *vm, AnimationFileView *file, int scriptId);

typedef char StageSpellRuntimeFileOffset[
    (offsetof(SpellStageRuntimeView, file) == 0x184ac) ? 1 : -1];
typedef char StageSpellEventActorOffset[
    (offsetof(SpellStageEventView, primaryActor) == 0x1c) ? 1 : -1];
typedef char StageSpellEventFileOffset[
    (offsetof(SpellStageEventView, file) == 0x48) ? 1 : -1];

namespace
{
__forceinline SpellStageEventView *EventOwner()
{
    return reinterpret_cast<SpellStageEventView *>(g_MeshEffectEvent);
}
__forceinline SecondarySpellStageView *StageOwner()
{
    return reinterpret_cast<SecondarySpellStageView *>(g_MeshEffectSpell);
}

// The original copies all characters, including the terminator. The player
// data file controls spell-name length; no bound check was present natively.
__forceinline void CopyStageSpellName(char *output, const char *name)
{
    char value;
    do
    {
        value = *name++;
        *output++ = value;
    } while (value);
}

__forceinline void RecordStageSpellName(char *record, const char *name)
{
    CopyStageSpellName(record, name);
    int &count = *reinterpret_cast<int *>(record + 0x84);
    if (count < 99999)
        ++count;
}

// This owner writes fraction, current, previous in native order. Keep this
// local rather than changing the shared timer helper used by other owners.
__forceinline void SetSpellStageTimerFrame(StageTimerView &timer, int frame)
{
    if (!(timer.flags & 1))
    {
        timer.fraction = 0.0f;
        timer.current = 0;
        timer.previous = -999999;
        timer.scale = &g_AnimationTimeScale;
        timer.flags |= 1;
    }
    timer.fraction = 0.0f;
    timer.current = 0;
    timer.previous = frame;
}

__forceinline void BindStageSpellVm(
    AnimationVmView *vm, AnimationFileView *file, int script)
{
    ResetAnimationVm(vm);
    unsigned char *bytes = reinterpret_cast<unsigned char *>(vm);
    bytes[0x41d] = 0x10;
    bytes[0x41c] = 0x10;
    BindAnimationScript(file, script, vm);
}

__forceinline void CreateStageSpellOverlayVm(SpellStageEventView *event, int script)
{
    CreateAnimationVmId(event->file, script, 0x16);
}

// The native searches the embedded draw-node list by the child VM's
// signed script ID. A missing child is not silently substituted by
// an unrelated current VM or a fake default animation.
__forceinline AnimationVmView *FindStageSpellChild(
    AnimationVmView *root, short scriptId)
{
    for (AnimationVmNode *node = &root->drawNode; node; node = node->next)
        if (static_cast<short>(node->vm->scriptId) == scriptId)
            return node->vm;
    return 0;
}

__forceinline void SetStageSpellChildDuration(
    AnimationVmView *root, short scriptId, int duration)
{
    AnimationVmView *child = FindStageSpellChild(root, scriptId);
    // The original stores through the result without adding a guard.
    child->duration3bc = duration;
}
}

// Complete native TH11 0x40C650..0x40CD40 spell-stage initializer.
// The original VC9 /GL caller passes the string in EAX and two integer
// arguments on the stack; the source signature leaves that private
// calling convention for a real whole-caller LTCG compilation.
void __stdcall InitializeSecondarySpellStage(
    const char *spellName, int spellIndex, int duration)
{
    SecondarySpellStageView *stage = StageOwner();
    SpellStageEventView *event = EventOwner();

    SetSpellStageTimerFrame(stage->time, -1);
    stage->spellIndex = spellIndex;
    CopyStageSpellName(stage->name, spellName);
    stage->flags = (stage->flags & ~0x18u) | 3u;

    if (g_StageSpellRecordStatus->mode != 1)
    {
        const int category = g_MeshEffectSelection0 * 3 + g_MeshEffectSelection1;
        const int spellOffset = spellIndex * 0x90;
        char *first = reinterpret_cast<char *>(
            g_StageSpellRecordData + category * 0x68d4 + spellOffset + 0x66c);
        RecordStageSpellName(first, spellName);
        char *second = reinterpret_cast<char *>(
            g_StageSpellRecordData + spellOffset + 0x27b64);
        RecordStageSpellName(second, spellName);
    }

    stage->flags &= ~0x20u;
    if (reinterpret_cast<StageAuxiliaryView *>(g_MeshEffectEnemy)->effectActive &&
        g_MeshEffectSelection0 * 3 + g_MeshEffectSelection1 != 5)
        stage->flags |= 0x20u;
    stage->flags &= ~0x40u;
    stage->activeFrames = 1;

    stage->followerIds[0] =
        CreateAnimationVmId(g_StageSpellRuntime->file, 1, 0x16).value;
    stage->followerIds[1] =
        CreateAnimationVmId(g_StageSpellOverlayFile, 0x4a, 0x16).value;
    stage->followerIds[2] =
        CreateAnimationVmId(g_StageSpellRuntime->file, 2, 0x16).value;

    AnimationVmView *nameVm = FindAnimationVmById(
        g_RenderBatch, AnimationVmId(stage->followerIds[1]));
    if (!nameVm)
        stage->followerIds[1] = 0;
    FormatRightAlignedTextureText(
        reinterpret_cast<FormattedTextVmView *>(nameVm),
        0xffffffu, 0, 0, 0, spellName);
    QueueMeshEffectSound(14);

    stage->positionVmId = CreateAnimationVmId(
        g_SecondaryActorFiles->file, 0x8b, 0x16).value;
    stage->position = event->primaryActor->state.world.position;
    PositionSecondaryAnimation(AnimationVmId(stage->positionVmId),
        &stage->position);

    AnimationVmView *effectVm = FindAnimationVmById(
        g_RenderBatch, AnimationVmId(stage->positionVmId));
    if (!effectVm)
        stage->positionVmId = 0;
    SetStageSpellChildDuration(effectVm, 0x89, duration);
    effectVm = FindAnimationVmById(
        g_RenderBatch, AnimationVmId(stage->positionVmId));
    if (!effectVm)
        stage->positionVmId = 0;
    SetStageSpellChildDuration(effectVm, 0x8a, duration);

    stage->scoreDuration = duration;
    stage->score = (g_StageSpellLevel + g_StageSpellDifficulty) * 1000000;
    stage->scoreInitial = stage->score;
    if (stage->scoreInitial >= 100000000)
        stage->scoreInitial = 99999999;

    CreateAnimationVmId(g_SecondaryActorFiles->file, 0x95, 0x16);

    AnimationFileView *file = event->file;
    int overlayScript = 0xa;
    switch (g_StageSpellDifficulty)
    {
    case 1:
        BindStageSpellVm(&stage->vm0, file, 7);
        BindStageSpellVm(&stage->vm1, file, 8);
        if (spellIndex < 2)
        {
            CreateStageSpellOverlayVm(event, 0xb);
            return;
        }
        break;
    case 2:
        BindStageSpellVm(&stage->vm0, file, 7);
        BindStageSpellVm(&stage->vm1, file, 8);
        break;
    case 3:
        BindStageSpellVm(&stage->vm0, file, 8);
        BindStageSpellVm(&stage->vm1, file, 9);
        CreateStageSpellOverlayVm(event, 0xb);
        return;
    case 4:
        BindStageSpellVm(&stage->vm0, file, 0xc);
        BindStageSpellVm(&stage->vm1, file, 0xd);
        CreateStageSpellOverlayVm(event, 0xf);
        return;
    case 5:
        BindStageSpellVm(&stage->vm0, file, 0x10);
        BindStageSpellVm(&stage->vm1, file, 0x11);
        if (spellIndex < 0x7a)
            return;
        overlayScript = 0x13;
        break;
    case 6:
        if (g_StageSpellProgress < 0x18)
        {
            ResetAndBindStageAnimationVm(&stage->vm0, file, 0x15);
            ResetAndBindStageAnimationVm(&stage->vm1, file, 0x16);
            CreateStageSpellOverlayVm(event, 0x17);
            return;
        }
        ResetAndBindStageAnimationVm(&stage->vm0, file, 0x12);
        ResetAndBindStageAnimationVm(&stage->vm1, file, 0x11);
        overlayScript = 0x14;
        break;
    case 7:
        if (g_StageSpellProgress < 0x18)
        {
            ResetAndBindStageAnimationVm(&stage->vm0, file, 0xb);
            ResetAndBindStageAnimationVm(&stage->vm1, file, 0xc);
            CreateStageSpellOverlayVm(event, 0xd);
            return;
        }
        ResetAndBindStageAnimationVm(&stage->vm0, file, 7);
        ResetAndBindStageAnimationVm(&stage->vm1, file, 8);
        break;
    default:
        return;
    }
    CreateStageSpellOverlayVm(event, overlayScript);
}
