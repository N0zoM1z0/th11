#include "SecondaryDialogue.hpp"
#include "StageMesh.hpp"
#include <stdlib.h>
#include <string.h>

namespace
{
using SecondaryHostInstructions::AddInteger;
__forceinline int Multiply(int left, int right)
{ return static_cast<int>(static_cast<unsigned int>(left) * right); }
__forceinline int &StateInteger(unsigned int offset)
{
    return *reinterpret_cast<int *>(
        reinterpret_cast<unsigned char *>(&g_SecondaryHostSelectionState) + offset);
}
__forceinline unsigned int ScriptInput(unsigned int offset)
{
    return *reinterpret_cast<unsigned int *>(
        reinterpret_cast<unsigned char *>(&g_InputState) + offset);
}
__forceinline int Argument(SecondaryDialogueView *self)
{ return *reinterpret_cast<int *>(self->instruction + 1); }
__forceinline AnimationFileView *PlayerFile()
{
    return *reinterpret_cast<AnimationFileView **>(
        reinterpret_cast<unsigned char *>(g_SecondaryActorReferences) + 0x10);
}
__forceinline AnimationFileView *HudFile(unsigned int offset)
{
    return *reinterpret_cast<AnimationFileView **>(
        reinterpret_cast<unsigned char *>(g_SecondaryActorHud) + offset);
}
__forceinline AnimationVmView *RequiredVm(RenderBatchView *renderer, unsigned int *id)
{
    AnimationVmView *vm = FindAnimationVmById(renderer, AnimationVmId(*id));
    if (!vm) *id = 0;
    return vm;
}
__forceinline float &TextOffset(AnimationVmView *vm)
{ return *reinterpret_cast<float *>(reinterpret_cast<unsigned char *>(vm) + 0x3f8); }
__forceinline void DrawText(SecondaryDialogueView *self, AnimationVmView *vm,
    int x, int font, int spacing, const char *text)
{
    FormatTextureText(reinterpret_cast<FormattedTextVmView *>(vm),
        self->colors[self->speaker], 0, x, font, spacing, text);
}
__forceinline void SignalSeven(AnimationVmView *vm)
{
    *reinterpret_cast<short *>(reinterpret_cast<unsigned char *>(vm) + 0x37c) = 7;
    if (!vm->drawNode.previous)
        for (AnimationVmNode *node = vm->drawNode.next; node; node = node->next)
            *reinterpret_cast<short *>(reinterpret_cast<unsigned char *>(node->vm) + 0x37c) = 7;
}
__forceinline SecondaryGameplayHudView *Hud()
{ return reinterpret_cast<SecondaryGameplayHudView *>(g_SecondaryActorHud); }
__forceinline int &HudInteger(void *hud, unsigned int offset)
{ return *reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(hud) + offset); }
}

int __stdcall RunSecondaryDialogue(SecondaryDialogueView *self)
{
    if (self->delay > 0) --self->delay;
    if ((self->flags & 1) && (ScriptInput(0x118) & 0x200))
    {
        int frame = self->instruction->frame;
        StageTimerView &timer = self->time;
        if (!(timer.flags & 1))
        {
            timer.fraction = 0.0f;
            timer.current = 0;
            timer.previous = -999999;
            timer.scale = &g_AnimationTimeScale;
            timer.flags |= 1;
        }
        timer.current = frame;
        timer.previous = AddInteger(frame, -1);
        timer.fraction = static_cast<float>(frame);
    }
    while (self->time.current >= self->instruction->frame)
    {
        // Keep the reviewed entry groups; opcode numbers are the wire values.
        switch (self->instruction->opcode)
        {
        case 26:
            self->flags |= 2;
            break;
        case 15:
        {
            AnimationVmView *vm = RequiredVm(g_RenderBatch, &self->text0);
            char *text = DecodeSecondaryDialogueText(reinterpret_cast<unsigned char *>(self->instruction + 1));
            DrawText(self, vm, 0, (self->flags >> 1) & 1, 0, text);
            SignalSecondaryProjectileHit(&self->text0);
            break;
        }
        case 16:
        {
            AnimationVmView *vm = RequiredVm(g_RenderBatch, &self->text1);
            char *text = DecodeSecondaryDialogueText(reinterpret_cast<unsigned char *>(self->instruction + 1));
            DrawText(self, vm, 0, (self->flags >> 1) & 1, 0, text);
            SignalSecondaryProjectileHit(&self->text1);
            break;
        }
        case 17:
        {
            char *text;
            int x, spacing;
            unsigned int *annotation;
            if (self->line == 0)
            {
                if (self->initialized == 0)
                {
                    AnimationVmView *text0 = RequiredVm(g_RenderBatch, &self->text0);
                    DrawText(self, text0, 0, (self->flags >> 1) & 1, 0, " ");
                    AnimationVmView *text1 = RequiredVm(g_RenderBatch, &self->text1);
                    DrawText(self, text1, 0, (self->flags >> 1) & 1, 0, " ");
                    AnimationVmView *annotation0 = RequiredVm(g_RenderBatch, &self->annotation0);
                    DrawText(self, annotation0, 0, (self->flags >> 1) & 1, 1, " ");
                    AnimationVmView *annotation1 = RequiredVm(g_RenderBatch, &self->annotation1);
                    DrawText(self, annotation1, 0, (self->flags >> 1) & 1, 1, " ");
                    self->initialized = 1;
                    ClearSecondaryProjectileHit(&self->text0);
                    ClearSecondaryProjectileHit(&self->text1);
                    ClearSecondaryProjectileHit(&self->annotation0);
                    ClearSecondaryProjectileHit(&self->annotation1);
                }
                text = DecodeSecondaryDialogueText(reinterpret_cast<unsigned char *>(self->instruction + 1));
                if (*text == '|')
                {
                    x = atoi(text + 1);
                    text = strchr(text + 1, ',');
                    spacing = atoi(text + 1);
                    text = strchr(text + 1, ',');
                    annotation = &self->annotation0;
                }
                else
                {
                    AnimationVmView *vm = RequiredVm(g_RenderBatch, &self->text0);
                    DrawText(self, vm, 0, (self->flags >> 1) & 1, 0, text);
                    SignalSecondaryProjectileHit(&self->text0);
                    self->line = AddInteger(self->line, 1);
                    break;
                }
            }
            else
            {
                text = DecodeSecondaryDialogueText(reinterpret_cast<unsigned char *>(self->instruction + 1));
                if (*text == '|')
                {
                    x = atoi(text + 1);
                    text = strchr(text + 1, ',');
                    spacing = atoi(text + 1);
                    text = strchr(text + 1, ',');
                    annotation = &self->annotation1;
                }
                else
                {
                    AnimationVmView *vm = RequiredVm(g_RenderBatch, &self->text1);
                    DrawText(self, vm, 0, (self->flags >> 1) & 1, 0, text);
                    SignalSecondaryProjectileHit(&self->text1);
                    self->line = self->initialized = 0;
                    break;
                }
            }
            AnimationVmView *vm = RequiredVm(g_RenderBatch, annotation);
            DrawText(self, vm, x, 2, spacing, text + 1);
            SignalSecondaryProjectileHit(annotation);
            break;
        }
        case 18:
            ClearSecondaryProjectileHit(&self->text0);
            ClearSecondaryProjectileHit(&self->text1);
            ClearSecondaryProjectileHit(&self->annotation0);
            ClearSecondaryProjectileHit(&self->annotation1);
            break;
        case 3:
            self->panel = CreateAnimationVmId(HudFile(0x444c), 0x2d, 0x16).value;
            break;
        case 1:
            if (g_MeshEffectSelection0 == 0)
                self->player = CreateAnimationVmId(PlayerFile(), 0x33, 0x16).value;
            else if (g_MeshEffectSelection0 == 1)
                self->player = CreateAnimationVmId(PlayerFile(), 0x47, 0x16).value;
            break;
        case 2:
            if (g_StageVariant == 5)
            {
                if (g_MeshEffectSelection0 == 0)
                    self->opponent = CreateAnimationVmId(PlayerFile(), 0x35, 0x16).value;
                else if (g_MeshEffectSelection0 == 1)
                    self->opponent = CreateAnimationVmId(PlayerFile(), 0x49, 0x16).value;
            }
            else if (g_StageVariant == 6)
            {
                if (g_MeshEffectSelection0 == 0)
                {
                    if (self->mode == 2)
                        self->opponent = CreateAnimationVmId(PlayerFile(), 0x35, 0x16).value;
                    else self->opponent = CreateAnimationVmId(PlayerFile(), 0x36, 0x16).value;
                }
                else if (g_MeshEffectSelection0 == 1)
                {
                    if (self->mode == 2)
                        self->opponent = CreateAnimationVmId(PlayerFile(), 0x49, 0x16).value;
                    else self->opponent = CreateAnimationVmId(PlayerFile(), 0x4a, 0x16).value;
                }
            }
            else if (g_MeshEffectSelection0 == 0)
                self->opponent = CreateAnimationVmId(PlayerFile(), 0x34, 0x16).value;
            else if (g_MeshEffectSelection0 == 1)
                self->opponent = CreateAnimationVmId(PlayerFile(), 0x48, 0x16).value;
            break;
        case 25:
        {
            RenderBatchView *renderer = g_RenderBatch;
            AnimationVmView *vm0 = RequiredVm(renderer, &self->text0);
            TextOffset(vm0) = static_cast<float>(Argument(self));
            AnimationVmView *vm1 = RequiredVm(renderer, &self->text1);
            TextOffset(vm1) = static_cast<float>(Argument(self));
            AnimationVmView *vm2 = RequiredVm(renderer, &self->annotation0);
            TextOffset(vm2) = static_cast<float>(Argument(self));
            AnimationVmView *vm3 = RequiredVm(renderer, &self->annotation1);
            TextOffset(vm3) = static_cast<float>(Argument(self));
            break;
        }
        case 23:
        {
            AnimationVmView *vm = FindAnimationVmById(g_RenderBatch, AnimationVmId(self->player));
            if (vm) SignalSeven(vm);
            break;
        }
        case 24:
        {
            AnimationVmView *vm = FindAnimationVmById(g_RenderBatch, AnimationVmId(self->opponent));
            if (vm) SignalSeven(vm);
            break;
        }
        case 4:
            SignalSecondaryAnimation(&self->player);
            self->player = 0;
            break;
        case 5:
            SignalSecondaryAnimation(&self->opponent);
            self->opponent = 0;
            SignalSecondaryAnimation(&self->boss);
            break;
        case 6:
            SignalSecondaryAnimation(&self->panel);
            SignalSecondaryAnimation(&self->text0);
            SignalSecondaryAnimation(&self->text1);
            SignalSecondaryAnimation(&self->annotation0);
            SignalSecondaryAnimation(&self->annotation1);
            break;
        case 7:
        {
            ClearSecondaryProjectileHit(&self->opponent);
            SignalSecondaryProjectileHit(&self->player);
            SignalSecondaryProjectileHit(&self->panel);
            self->speaker = 0;
            PositionSecondaryAnimationRaw(AnimationVmId(self->text0), &self->positions[0]);
            PositionSecondaryAnimationRaw(AnimationVmId(self->text1), &self->positions[self->speaker]);
            RenderBatchView *renderer = g_RenderBatch;
            TextOffset(RequiredVm(renderer, &self->text0)) = 0.0f;
            TextOffset(RequiredVm(renderer, &self->text1)) = 0.0f;
            PositionSecondaryAnimationRaw(AnimationVmId(self->annotation0), &self->positions[self->speaker]);
            PositionSecondaryAnimationRaw(AnimationVmId(self->annotation1), &self->positions[self->speaker]);
            TextOffset(RequiredVm(renderer, &self->annotation0)) = 0.0f;
            TextOffset(RequiredVm(renderer, &self->annotation1)) = 0.0f;
            self->flags &= ~2u;
            self->line = self->initialized = 0;
            break;
        }
        case 8:
        {
            ClearSecondaryProjectileHit(&self->player);
            SignalSecondaryProjectileHit(&self->opponent);
            ClearSecondaryProjectileHit(&self->panel);
            self->speaker = 1;
            PositionSecondaryAnimationRaw(AnimationVmId(self->text0), &self->positions[1]);
            PositionSecondaryAnimationRaw(AnimationVmId(self->text1), &self->positions[self->speaker]);
            RenderBatchView *renderer = g_RenderBatch;
            TextOffset(RequiredVm(renderer, &self->text0)) = 0.0f;
            TextOffset(RequiredVm(renderer, &self->text1)) = 0.0f;
            PositionSecondaryAnimationRaw(AnimationVmId(self->annotation0), &self->positions[self->speaker]);
            PositionSecondaryAnimationRaw(AnimationVmId(self->annotation1), &self->positions[self->speaker]);
            TextOffset(RequiredVm(renderer, &self->annotation0)) = 0.0f;
            TextOffset(RequiredVm(renderer, &self->annotation1)) = 0.0f;
            self->flags &= ~2u;
            self->line = self->initialized = 0;
            break;
        }
        case 9:
        {
            SignalSecondaryAnimationEvent(AnimationVmId(self->opponent), 8);
            SignalSecondaryAnimationEvent(AnimationVmId(self->player), 8);
            SignalSecondaryAnimationEvent(AnimationVmId(self->panel), 8);
            self->speaker = 2;
            PositionSecondaryAnimationRaw(AnimationVmId(self->text0), &self->positions[2]);
            PositionSecondaryAnimationRaw(AnimationVmId(self->text1), &self->positions[self->speaker]);
            RenderBatchView *renderer = g_RenderBatch;
            TextOffset(RequiredVm(renderer, &self->text0)) = 0.0f;
            TextOffset(RequiredVm(renderer, &self->text1)) = 0.0f;
            PositionSecondaryAnimationRaw(AnimationVmId(self->annotation0), &self->positions[self->speaker]);
            PositionSecondaryAnimationRaw(AnimationVmId(self->annotation1), &self->positions[self->speaker]);
            TextOffset(RequiredVm(renderer, &self->annotation0)) = 0.0f;
            TextOffset(RequiredVm(renderer, &self->annotation1)) = 0.0f;
            self->flags |= 2;
            self->line = self->initialized = 0;
            break;
        }
        case 10:
            self->flags ^= (static_cast<int>(*reinterpret_cast<signed char *>(self->instruction + 1)) ^ self->flags) & 1u;
            break;
        case 13:
        {
            if (g_MeshEffectSelection0 == 0)
            {
                ChangeSecondaryDialogueSprite(FindSecondaryDialogueChild(&self->player, 0x27),
                    AddInteger(Argument(self), 0x40));
                ChangeSecondaryDialogueSprite(FindSecondaryDialogueChild(&self->player, 0x2b),
                    AddInteger(Argument(self), 0x48));
                ChangeSecondaryDialogueSprite(FindSecondaryDialogueChild(&self->player, 0x2f),
                    AddInteger(Argument(self), 0x50));
            }
            else if (g_MeshEffectSelection0 == 1)
            {
                ChangeSecondaryDialogueSprite(FindSecondaryDialogueChild(&self->player, 0x3b),
                    AddInteger(Argument(self), 0x4d));
                ChangeSecondaryDialogueSprite(FindSecondaryDialogueChild(&self->player, 0x3f),
                    AddInteger(Argument(self), 0x55));
                ChangeSecondaryDialogueSprite(FindSecondaryDialogueChild(&self->player, 0x43),
                    AddInteger(Argument(self), 0x5d));
            }
            break;
        }
        case 14:
#include "SecondaryDialoguePortraits.inc"
            break;
        case 11:
            if (self->wait.current <= 0) SetStageTimer(&self->wait, Argument(self));
            OffsetSecondaryTimer(&self->wait, -1.0f);
            if (!(ScriptInput(0x124) & 0x80001) && self->wait.current > 0)
            {
                if (!(self->flags & 1)) return 0;
                if (!(ScriptInput(0x118) & 0x200)) return 0;
                SetStageTimer(&self->wait, 0);
                self->line = self->initialized = 0;
            }
            else
            {
                QueueMeshEffectSound(0);
                SetStageTimer(&self->wait, 0);
                self->line = self->initialized = 0;
            }
            break;
        case 12:
            self->delay = 1;
            break;
        case 19:
            StartSecondaryGameplayAudio(1, *reinterpret_cast<int *>(
                reinterpret_cast<unsigned char *>(g_SecondaryGameplaySelectionEC8) + 0x38));
            CreateAnimationVmId(HudFile(0x43ec), 2, 0x16);
            break;
        case 20:
            switch (g_StageVariant)
            {
            case 1: self->boss = CreateAnimationVmId(SecondaryActorFile(0x48), 9, 0x16).value; break;
            case 2: self->boss = CreateAnimationVmId(SecondaryActorFile(0x48), 9, 0x16).value; break;
            case 3: self->boss = CreateAnimationVmId(SecondaryActorFile(0x48), 10, 0x16).value; break;
            case 4: self->boss = CreateAnimationVmId(SecondaryActorFile(0x48), 14, 0x16).value; break;
            case 5: self->boss = CreateAnimationVmId(SecondaryActorFile(0x48), 18, 0x16).value; break;
            case 6: self->boss = CreateAnimationVmId(SecondaryActorFile(0x48), 19, 0x16).value; break;
            case 7: self->boss = CreateAnimationVmId(SecondaryActorFile(0x48), 9, 0x16).value; break;
            }
            break;
        case 21:
#include "SecondaryDialogueClear.inc"
            break;
        case 22:
            if (g_StageVariant == 6) FadeSecondaryDialogueAudio(8.0f);
            else FadeSecondaryDialogueAudio(2.0f);
            break;
        case 0:
            if (g_SecondaryHostSelectionState.selection)
                g_SecondaryHostSelectionState.value058 = 0;
            g_SecondaryHostSelectionState.selection = 0;
            return -1;
        }
        self->instruction = reinterpret_cast<SecondaryDialogueInstruction *>(
            reinterpret_cast<unsigned char *>(self->instruction + 1) + self->instruction->payloadBytes);
    }
    AdvanceStageTimer(&self->time);
    return 0;
}
