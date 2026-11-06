#include "ConfigurationAudioMenu.hpp"
#include "AnimationSpriteBinding.hpp"
#include "AnimationVmFactory.hpp"
#include "InputBackend.hpp"
#include "MainRuntime.hpp"
#include "MeshEffects.hpp"
#include "SecondaryDamage.hpp"
#include "SecondaryPlayerMovement.hpp"
#include "SoundQueue.hpp"
#include "VmBindings.hpp"

namespace
{
__forceinline signed char &Setting(unsigned int index)
{
    return reinterpret_cast<signed char *>(&g_MainRuntime)[0x1e8 + index];
}

__forceinline AnimationVmId ChildId(unsigned int *parentId,
    RenderBatchView *renderer, int script)
{
    AnimationVmView *parent = FindAnimationVmById(renderer, AnimationVmId(*parentId));
    if (!parent) *parentId = 0;
    for (AnimationVmNode *node = &parent->drawNode; node; node = node->next)
        if (static_cast<short>(node->vm->scriptId) == script)
            return AnimationVmId(node->vm->unknown000);
    return AnimationVmId(0);
}

__forceinline AnimationVmView *Child(ConfigurationAudioMenuView *self,
    RenderBatchView *renderer, int script)
{
    AnimationVmId id = ChildId(&self->rootVmId, renderer, script);
    return FindAnimationVmById(renderer, id);
}

__forceinline void Digit(ConfigurationAudioMenuView *self, RenderBatchView *renderer,
    int script, unsigned int setting, int divisor, int sprite)
{
    AnimationVmView *vm = Child(self, renderer, script);
    if (vm)
    {
        int value;
        if (divisor == 100) value = Setting(setting) / 100;
        else if (divisor == 10) value = Setting(setting) / 10 % 10;
        else value = Setting(setting) % 10;
        BindAnimationSprite(reinterpret_cast<AnimationDocumentView *>(vm->file),
            value + sprite, vm);
    }
}

__forceinline void Hide(ConfigurationAudioMenuView *self, RenderBatchView *renderer, int script)
{
    Child(self, renderer, script)->flags.value &= ~2u;
}

__forceinline void Show(ConfigurationAudioMenuView *self, RenderBatchView *renderer, int script)
{
    Child(self, renderer, script)->flags.value |= 2;
}

__forceinline void Select(SecondaryDebugSelectionView *selection, int value)
{
    if (!selection->count) selection->current = value;
    else if (selection->count <= value) selection->current = selection->count - 1;
    else selection->current = value;
}

__forceinline void Close(ConfigurationAudioMenuView *self, int sound)
{
    AnimationVmView *vm = FindAnimationVmById(g_RenderBatch, AnimationVmId(self->rootVmId));
    if (vm)
    {
        reinterpret_cast<SecondaryActorAnimationView *>(vm)->event = 6;
        BoundVmView *bound = reinterpret_cast<BoundVmView *>(vm);
        if (!bound->marker018)
            for (BoundVmNodeView *node = bound->related; node; node = node->next)
                reinterpret_cast<SecondaryActorAnimationView *>(node->vm)->event = 6;
    }
    QueueMeshEffectSound(sound);
    SetConfigurationMenuState(self, 4);
}
}

void __stdcall ApplyConfigurationAudioMenu(ConfigurationAudioMenuView *self)
{
    g_MusicVolume = Setting(0);
    EnqueueSoundCommand(&g_SoundManagerQueue, 8, 0, "SetVol");
    g_SoundEffectVolume = Setting(1);
    if (g_SoundEffectVolume)
    {
        float gain = static_cast<float>(g_MusicVolume / 100.0);
        gain = static_cast<float>(1.0 - gain);
        gain = static_cast<float>(static_cast<double>(gain) * gain);
        gain = static_cast<float>(static_cast<double>(gain) * gain);
        gain = static_cast<float>(1.0 - gain);
        g_ConfigurationSoundAttenuation = -5000 - static_cast<int>(gain * -5000.0);
    }
    else g_ConfigurationSoundAttenuation = -10000;

    RenderBatchView *renderer = g_RenderBatch;
    Digit(self, renderer, 0x1d, 0, 100, 0x2a);
    Digit(self, renderer, 0x1e, 0, 10, 0x2a);
    Digit(self, renderer, 0x1f, 0, 1, 0x2a);
    Digit(self, renderer, 0x21, 0, 100, 0x34);
    Digit(self, renderer, 0x22, 0, 10, 0x34);
    Digit(self, renderer, 0x23, 0, 1, 0x34);
    Digit(self, renderer, 0x25, 1, 100, 0x2a);
    Digit(self, renderer, 0x26, 1, 10, 0x2a);
    Digit(self, renderer, 0x27, 1, 1, 0x2a);
    Digit(self, renderer, 0x29, 1, 100, 0x34);
    Digit(self, renderer, 0x2a, 1, 10, 0x34);
    Digit(self, renderer, 0x2b, 1, 1, 0x34);

    if (Setting(0) < 10)
    {
        Hide(self, renderer, 0x1d); Hide(self, renderer, 0x1e);
        Hide(self, renderer, 0x21); Hide(self, renderer, 0x22);
    }
    else if (Setting(0) < 100)
    {
        Hide(self, renderer, 0x1d); Show(self, renderer, 0x1e);
        Hide(self, renderer, 0x21); Show(self, renderer, 0x22);
    }
    else
    {
        Show(self, renderer, 0x1d); Show(self, renderer, 0x1e);
        Show(self, renderer, 0x21); Show(self, renderer, 0x22);
    }
    if (Setting(1) < 10)
    {
        Hide(self, renderer, 0x25); Hide(self, renderer, 0x26);
        Hide(self, renderer, 0x29); Hide(self, renderer, 0x2a);
    }
    else if (Setting(1) < 100)
    {
        Hide(self, renderer, 0x25); Show(self, renderer, 0x26);
        Hide(self, renderer, 0x29); Show(self, renderer, 0x2a);
    }
    else
    {
        Show(self, renderer, 0x25); Show(self, renderer, 0x26);
        Show(self, renderer, 0x29); Show(self, renderer, 0x2a);
    }
}

void __stdcall SetConfigurationMenuState(ConfigurationAudioMenuView *self, int state)
{
    self->state = state;
    self->time.SetFrame(-1);
}

void __stdcall SetConfigurationMenuPage(ConfigurationAudioMenuView *self, int page)
{
    self->page = page;
    self->state = 0;
    self->time.SetFrame(-1);
}

int __stdcall UpdateConfigurationAudioMenu(ConfigurationAudioMenuView *self)
{
    switch (self->state)
    {
    case 0:
        self->selection.count = 5;
        Select(&self->selection, 0);
        self->rootVmId = CreateAnimationVmId(self->file, 1, 0x16).value;
        ApplyConfigurationAudioMenu(self);
        SetConfigurationMenuState(self, 1);
        // Fall through to the opening animation delay.
    case 1:
        if (self->time.current > 6)
        {
            SetConfigurationMenuState(self, 2);
            SignalAndExecuteConfigurationAnimation(AnimationVmId(self->rootVmId), 3);
            SignalSecondaryAnimationEvent(AnimationVmId(self->rootVmId),
                static_cast<short>(self->selection.current + 0x11));
        }
        break;
    case 2:
        self->selection.previous = self->selection.current;
        if ((g_InputState.pressed & 0x10) || (g_InputState.repeat & 0x10))
            MoveSecondaryDebugSelection(&self->selection, -1);
        if ((g_InputState.pressed & 0x20) || (g_InputState.repeat & 0x20))
            MoveSecondaryDebugSelection(&self->selection, 1);
        if (self->selection.previous != self->selection.current)
        {
            QueueMeshEffectSound(12);
            SignalAndExecuteConfigurationAnimation(AnimationVmId(self->rootVmId), 3);
            SignalSecondaryAnimationEvent(AnimationVmId(self->rootVmId),
                static_cast<short>(self->selection.current + 7));
        }
        if (g_InputState.pressed & 0x102)
        {
            if (self->selection.current == 4) goto CloseBack;
            QueueMeshEffectSound(11);
            Select(&self->selection, 4);
            SignalAndExecuteConfigurationAnimation(AnimationVmId(self->rootVmId), 3);
            SignalSecondaryAnimationEvent(AnimationVmId(self->rootVmId),
                static_cast<short>(self->selection.current + 7));
            return 1;
        }
        if (self->selection.current == 1 &&
            reinterpret_cast<SecondaryDamageTimerView *>(&self->time)->Elapsed(60))
            QueueMeshEffectSound(4);
        if ((g_InputState.pressed & 0x40) || (g_InputState.repeat & 0x40))
        {
            switch (self->selection.current)
            {
            case 0:
                if (Setting(0) < 5) Setting(0) = 0; else Setting(0) -= 5;
                ApplyConfigurationAudioMenu(self);
                break;
            case 1:
                if (Setting(1) < 5) Setting(1) = 0; else Setting(1) -= 5;
                ApplyConfigurationAudioMenu(self);
                break;
            }
        }
        if ((g_InputState.pressed & 0x80) || (g_InputState.repeat & 0x80))
        {
            switch (self->selection.current)
            {
            case 0:
                Setting(0) += 5;
                if (Setting(0) > 100) Setting(0) = 100;
                ApplyConfigurationAudioMenu(self);
                break;
            case 1:
                Setting(1) += 5;
                if (Setting(1) > 100) Setting(1) = 100;
                ApplyConfigurationAudioMenu(self);
                break;
            }
        }
        if (g_InputState.pressed & 0x80001)
        {
            switch (self->selection.current)
            {
            case 4:
            CloseBack:
                Close(self, 11);
                return 1;
            case 3:
                Setting(0) = 100;
                Setting(1) = 80;
                Setting(2) = 0;
                ApplyConfigurationAudioMenu(self);
                QueueMeshEffectSound(10);
                return 1;
            case 2: Close(self, 10); return 1;
            }
        }
        break;
    case 3:
        break;
    case 4:
        if (self->time.current > 9)
        {
            switch (self->selection.current)
            {
            case 2:
                SetConfigurationMenuPage(self, 4);
                PushConfigurationSelection(&self->selection);
                break;
            case 4:
                SetConfigurationMenuPage(self, 1);
                PopConfigurationSelection(&self->selection);
                return 1;
            }
        }
        break;
    }
    return 1;
}
