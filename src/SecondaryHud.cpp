#include "SecondaryHud.hpp"
#include "SecondaryPlayerController.hpp"
#include "StageMesh.hpp"
#include "MeshEffects.hpp"
#include <math.h>
#include <string.h>

namespace
{
using SecondaryHostInstructions::AddInteger;
__forceinline int Multiply(int left, int right)
{ return static_cast<int>(static_cast<unsigned int>(left) * right); }
__forceinline float &VmFloat(AnimationVmView *vm, unsigned int offset)
{ return *reinterpret_cast<float *>(reinterpret_cast<unsigned char *>(vm) + offset); }
__forceinline short &Event(void *vm)
{ return *reinterpret_cast<short *>(reinterpret_cast<unsigned char *>(vm) + 0x37c); }
__forceinline unsigned char &Alpha(AnimationVmView *vm)
{ return reinterpret_cast<unsigned char *>(vm)[0x377]; }
__forceinline void Signal(unsigned int id, short event)
{
    if (!id) return;
    BoundVmView *vm = FindBoundVm(id);
    if (!vm) return;
    Event(vm) = event;
    if (!vm->marker018)
        for (BoundVmNodeView *node = vm->related; node; node = node->next)
            Event(node->vm) = event;
}
__forceinline void Release(unsigned int &id)
{
    if (id)
    {
        BoundVmView *vm = FindBoundVm(id);
        if (vm)
        {
            vm->flags |= 0x04000000;
            if (!vm->marker018)
                for (BoundVmNodeView *node = vm->related; node; node = node->next)
                    node->vm->flags |= 0x04000000;
        }
    }
    id = 0;
}
__forceinline void Tick(StageTimerView *self)
{
    int previous = self->current;
    self->previous = previous;
    if (*self->scale > static_cast<double>(0.99f) && *self->scale < 1.01f)
    {
        self->current = AddInteger(previous, 1);
        self->fraction = static_cast<float>(self->fraction + 1.0);
    }
    else
    {
        self->fraction = *self->scale + self->fraction;
        self->current = static_cast<int>(self->fraction);
    }
}
__forceinline bool MasterBlocked(SecondaryMasterView *master)
{ return (*reinterpret_cast<unsigned int *>(master->indexed01c + (0x3c - 0x1c)) & 1) != 0; }
__forceinline void LockAnimations()
{
    if (g_MainRuntime.flags & 0x8000)
    {
        EnterCriticalSection(&g_MainRuntime.criticalSections[9]);
        ++g_MainRuntime.criticalDepth[9];
    }
}
__forceinline void UnlockAnimations()
{
    if (g_MainRuntime.flags & 0x8000)
    {
        LeaveCriticalSection(&g_MainRuntime.criticalSections[9]);
        --g_MainRuntime.criticalDepth[9];
    }
}
struct HudVmRegistryView
{
    unsigned char unknown000[0x7b562c];
    AnimationVmNode *first, *tail, *second;
    unsigned char unknown7b5638[0x7bd888 - 0x7b5638];
    unsigned int nextId;
};
__forceinline unsigned int PublishBadge(AnimationVmView *vm)
{
    HudVmRegistryView *registry = reinterpret_cast<HudVmRegistryView *>(g_RenderBatch);
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
    if (!registry->nextId) ++registry->nextId;
    vm->unknown000 = registry->nextId;
    return registry->nextId;
}
}

int __stdcall UpdateSecondaryHud(SecondaryHudView *self)
{
    if (self->flags & 0x200) AdvanceStageTimer(&self->transition);
    if (self->flags & 0x10)
    {
        self->clearFrame = AddInteger(self->clearFrame, 1);
        if (self->clearFrame == 180)
        {
            SecondaryHudScreenEffect *effect = new SecondaryHudScreenEffect;
            if (effect) InitializeSecondaryHudScreenEffect(effect, 5, 200, 0, 0, 0x43);
        }
        if (self->clearFrame >= 380)
        {
            if (g_SecondaryDialogueSession->mode) EnterSecondaryContinueMenu();
            else *reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(&g_MainRuntime) + 0x558) =
                (g_MainRuntime.flags & 0x2000) ? 2 : 15;
        }
    }
    if (self->trackedActive && !FindAnimationVmById(g_RenderBatch, AnimationVmId(self->tracked)))
    {
        self->tracked = 0;
        self->trackedActive = 0;
    }
    for (int i = 0; i < 9; ++i) ExecuteAnimationVm(&self->primary[i]);
    for (int i = 0; i < 2; ++i) ExecuteAnimationVm(&self->digits[i]);
    if (!(self->flags & 1))
    {
        if (g_SecondaryActorReferences && g_SecondaryActorReferences->position.y > 416.0 &&
            g_SecondaryActorReferences->position.x < -64.0f)
        {
            for (int i = 0; i < 4; ++i) Event(&self->gauges[i]) = 3;
            self->flags |= 1;
        }
    }
    else if (g_SecondaryActorReferences && (g_SecondaryActorReferences->position.y < 400.0 ||
        g_SecondaryActorReferences->position.x > -64.0f))
    {
        for (int i = 0; i < 4; ++i) Event(&self->gauges[i]) = 2;
        self->flags &= ~1u;
    }
    for (int i = 0; i < 4; ++i) ExecuteAnimationVm(&self->gauges[i]);
    int percent = Multiply(g_SecondaryHostSelectionState.gaugeValue014, 100) / 10000;
    if (self->time.current >= 20)
    {
        if (percent < 100)
        {
            if (self->flags & 0x1c0)
                {
                Event(&self->gauges[1]) = 7;
                Event(&self->gauges[2]) = 7;
            }
            self->flags &= ~0x1c0u;
        }
        else if (percent < 110)
        {
            if ((self->flags & 0x1c0) != 0x40)
                {
                Event(&self->gauges[1]) = 8;
                Event(&self->gauges[2]) = 8;
            }
            self->flags = (self->flags & ~0x180u) | 0x40;
        }
        else if (percent < 120)
        {
            if ((self->flags & 0x1c0) != 0x80)
                {
                Event(&self->gauges[1]) = 9;
                Event(&self->gauges[2]) = 9;
            }
            self->flags = (self->flags & ~0x140u) | 0x80;
        }
        else
        {
            if ((self->flags & 0x1c0) != 0xc0)
                {
                Event(&self->gauges[1]) = 10;
                Event(&self->gauges[2]) = 10;
            }
            self->flags = (self->flags & ~0x100u) | 0xc0;
        }
    }
    percent = g_SecondaryHostSelectionState.gaugeValue014 < 10000 ?
        Multiply(g_SecondaryHostSelectionState.gaugeValue014, 100) / 10000 : 100;
    int tens = percent / 10;
    float offset = static_cast<float>(tens * 4.0 * 0.001953125 + (VmFloat(&self->gauges[1], 0x70) + 0.017578125));
    VmFloat(&self->gauges[1], 0x78) = offset;
    VmFloat(&self->gauges[1], 0x88) = offset;
    self->gauges[1].flags.value |= 8;
    VmFloat(&self->gauges[1], 0x4c) = static_cast<float>(Multiply(tens, 4) + 9.0);
    self->gauges[2].flags.value |= 8;
    VmFloat(&self->gauges[2], 0x4c) = 0.0f;

    SecondaryMasterView *master = g_SecondaryMaster;
    if (master)
    {
        if (self->seconds >= 0 && master->Slot(0) && !MasterBlocked(master) && !self->dialogue)
        {
            if (self->seconds < self->previousSeconds)
            {
                if (self->seconds < 5)
                {
                    Event(&self->digits[0]) = 9;
                    Event(&self->digits[1]) = 9;
                    QueueMeshEffectSound(0x24);
                }
                else if (self->seconds < 10)
                {
                    Event(&self->digits[0]) = 8;
                    Event(&self->digits[1]) = 8;
                    QueueMeshEffectSound(0x1b);
                }
            }
            else if (self->seconds > self->previousSeconds)
            {
                Event(&self->digits[0]) = 7;
                Event(&self->digits[1]) = 7;
            }
            if (self->seconds != self->previousSeconds)
            {
                if (self->digits[0].file) BindAnimationSprite(
                    reinterpret_cast<AnimationDocumentView *>(self->digits[0].file), self->seconds / 10 + 0xf3, &self->digits[0]);
                if (self->digits[1].file) BindAnimationSprite(
                    reinterpret_cast<AnimationDocumentView *>(self->digits[1].file), self->seconds % 10 + 0xf3, &self->digits[1]);
            }
            self->previousSeconds = self->seconds;
        }
        SecondaryActorManagerView *boss = master->Slot(0);
        if (boss && !MasterBlocked(master) && !self->dialogue)
        {
            int value = boss->state.counter.value;
            self->bossValue = value;
            self->bossTarget = static_cast<float>(value) / boss->state.counter.unknown004;
            if (self->bossDisplayed < self->bossTarget)
                self->bossDisplayed = static_cast<float>(self->bossDisplayed + static_cast<double>(0.025f));
            if (self->bossTarget < self->bossDisplayed) self->bossDisplayed = self->bossTarget;
            if (self->flags & 8)
            {
                if (g_SecondaryActorReferences->position.y >= 80.0f || g_SecondaryActorReferences->position.x > 0.0f)
                {
                    for (int i = 0; i < self->badgeCount; ++i) Signal(self->badges[i], 2);
                    Signal(self->title, 2);
                    self->flags &= ~8u;
                }
            }
            else if (g_SecondaryActorReferences->position.y <= 64.0f && g_SecondaryActorReferences->position.x < -64.0f)
            {
                for (int i = 0; i < self->badgeCount; ++i) Signal(self->badges[i], 3);
                Signal(self->title, 3);
                self->flags |= 8;
            }
            if (!self->title)
            {
                short script = 0x69;
                switch (g_StageVariant)
                {
                case 1: if (g_SecondaryHostSelectionState.selection > 23) script = 0x6a; break;
                case 2: if (g_SecondaryHostSelectionState.selection < 24) goto secondary_hud_badges; script = 0x6b; break;
                case 3: if (g_SecondaryHostSelectionState.selection < 24) goto secondary_hud_badges; script = 0x6c; break;
                case 4: if (g_SecondaryHostSelectionState.selection < 24) goto secondary_hud_badges; script = 0x6d; break;
                case 5: if (g_SecondaryHostSelectionState.selection < 24) goto secondary_hud_badges; script = 0x6e; break;
                case 6: script = 0x6e + (g_SecondaryHostSelectionState.selection > 23); break;
                case 7: script = 0x70 + (g_SecondaryHostSelectionState.selection > 23); break;
                }
                AnimationFileView *file = self->portraitFile;
                LockAnimations();
                AnimationVmView *vm = AllocateSceneAnimationVm();
                vm->flags.value |= 0x40000000;
                vm->persistent020 = 0x16;
                InitializeSceneAnimationVm(vm, file, script);
                AnimationVmId id = PublishSceneAnimationVm(vm);
                UnlockAnimations();
                self->title = id.value;
            }
secondary_hud_badges:
            for (int i = 0; i < 10; ++i)
            {
                if (i < self->badgeCount)
                {
                    if (!self->badges[i])
                    {
                        AnimationFileView *file = self->portraitFile;
                        LockAnimations();
                        AnimationVmView *vm = AllocateSceneAnimationVm();
                        vm->flags.value |= 0x40000000;
                        vm->persistent020 = 0x16;
                        InitializeSceneAnimationVm(vm, file, static_cast<short>(i + 0x2e));
                        unsigned int id = PublishBadge(vm);
                        UnlockAnimations();
                        self->badges[i] = id;
                    }
                }
                else
                {
                    Signal(self->badges[i], 1);
                    self->badges[i] = 0;
                }
            }
            goto secondary_hud_dialogue;
        }
    }
    if (self->title)
    {
        Signal(self->title, 1);
        self->title = 0;
    }
    self->bossDisplayed = 0.0f;
    self->bars[0].fraction = 0.0f;
    self->bars[1].fraction = 0.0f;
    self->bars[2].fraction = 0.0f;
    self->bars[3].fraction = 0.0f;
secondary_hud_dialogue:
    SecondaryDialogueView *dialogue = self->dialogue;
    if (dialogue)
    {
        if (!RunSecondaryDialogue(dialogue)) Tick(&dialogue->elapsed);
        else
        {
            dialogue = self->dialogue;
            if (dialogue)
            {
                Release(dialogue->player);
                Release(dialogue->opponent);
                Release(dialogue->panel);
                Release(dialogue->text0);
                Release(dialogue->text1);
                Release(dialogue->annotation0);
                Release(dialogue->annotation1);
                Release(dialogue->boss);
                operator delete(dialogue);
            }
            self->dialogue = 0;
        }
        master = g_SecondaryMaster;
    }
    if (master && master->Slot(0) && !(master->Slot(0)->state.flags & 0x20) && !(master->Slot(0)->state.flags & 1))
    {
        SecondaryActorManagerView *boss = master->Slot(0);
        bool spell = (SecondaryActorSpell()->flags & 1) != 0;
        unsigned int state = (self->flags >> 1) & 3;
        int time = boss->state.counter.unknown008;
        if (spell)
        {
            if (state == 0 && time < 2000) { Event(&self->indicator) = 7; self->flags = (self->flags & ~4u) | 2; }
            else if (state == 1 && time < 1000) { Event(&self->indicator) = 8; self->flags = (self->flags & ~2u) | 4; }
            else if (state == 2 && time < 400) { Event(&self->indicator) = 9; self->flags |= 6; }
            else if (state == 3 && time > 400) { Event(&self->indicator) = 10; self->flags &= ~6u; }
        }
        else
        {
            if (state == 0 && time < 700) { Event(&self->indicator) = 7; self->flags = (self->flags & ~4u) | 2; }
            else if (state == 1 && time < 400) { Event(&self->indicator) = 8; self->flags = (self->flags & ~2u) | 4; }
            else if (state == 2 && time < 200) { Event(&self->indicator) = 9; self->flags |= 6; }
            else if (state == 3 && time > 200) { Event(&self->indicator) = 10; self->flags &= ~6u; }
        }
        ExecuteAnimationVm(&self->indicator);
        VmFloat(&self->indicator, 0x3e8) = static_cast<float>(boss->state.world.position.x + 32.0 + 192.0);
        VmFloat(&self->indicator, 0x3ec) = 480.0f;
        float distance = static_cast<float>(fabsf(boss->state.world.position.x - g_SecondaryActorReferences->position.x));
        if (distance >= 64.0f) Alpha(&self->indicator) = 0xff;
        else
        {
            signed char delta = static_cast<signed char>(distance * -191.0 * 0.015625);
            Alpha(&self->indicator) = static_cast<unsigned char>(0x40 - delta);
        }
        if (boss->state.world.position.x < -192.0f || boss->state.world.position.x > 192.0f)
            Alpha(&self->indicator) = 0;
    }
    Tick(&self->time);
    return 1;
}
