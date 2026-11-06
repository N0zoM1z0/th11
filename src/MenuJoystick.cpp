#include "MenuJoystick.hpp"
#include "AnimationVmFactory.hpp"
#include "JoystickButtons.hpp"
#include "MainRuntime.hpp"
#include "MeshEffects.hpp"
#include "SecondaryActorRuntime.hpp"
#include "VmBindings.hpp"

namespace
{
__forceinline void Select(SecondaryDebugSelectionView *selection, int value)
{
    if (selection->count)
        selection->current = selection->count <= value ?
            static_cast<int>(static_cast<unsigned int>(selection->count) - 1) : value;
    else selection->current = value;
}
__forceinline void LoadBindings(MenuControllerView *self)
{
    self->joystickBindings[0] = g_JoystickBindings.fire;
    self->joystickBindings[1] = g_JoystickBindings.bomb;
    self->joystickBindings[2] = g_JoystickBindings.slow;
    self->joystickBindings[3] = g_JoystickBindings.pause;
    self->joystickBindings[4] = g_JoystickBindings.skip;
}
__forceinline void SignalClose(AnimationVmId id, short event)
{
    AnimationVmView *vm = FindAnimationVmById(g_RenderBatch, id);
    if (vm)
    {
        reinterpret_cast<SecondaryActorAnimationView *>(vm)->event = event;
        BoundVmView *bound = reinterpret_cast<BoundVmView *>(vm);
        if (!bound->marker018)
            for (BoundVmNodeView *node = bound->related; node; node = node->next)
                reinterpret_cast<SecondaryActorAnimationView *>(node->vm)->event = event;
    }
}
}

int __stdcall UpdateMenuPage04(MenuControllerView *self)
{
    if (static_cast<unsigned int>(self->page.state) > 4) return 1;
    switch (self->page.state)
    {
    case 0:
        self->page.selection.count = 7;
        Select(&self->page.selection, 0);
        self->joystickRootVmId = CreateAnimationVmId(self->page.file, 2, 0x16);
        SetConfigurationMenuState(&self->page, 1);
        LoadBindings(self);
        ApplyMenuJoystickBindings(self);
        // Fall through to the opening delay.
    case 1:
        if (self->page.time.current > 6)
        {
            SetConfigurationMenuState(&self->page, 2);
            SignalAndExecuteConfigurationAnimation(self->joystickRootVmId, 3);
            SignalSecondaryAnimationEvent(self->joystickRootVmId,
                static_cast<short>(self->page.selection.current + 0x11));
        }
        break;
    case 2:
    {
        SecondaryDebugSelectionView *selection = &self->page.selection;
        selection->previous = selection->current;
        if ((g_InputState.pressed & 0x10) || (g_InputState.repeat & 0x10))
            MoveSecondaryDebugSelection(selection, -1);
        if ((g_InputState.pressed & 0x20) || (g_InputState.repeat & 0x20))
            MoveSecondaryDebugSelection(selection, 1);
        if (selection->previous != selection->current)
        {
            QueueMeshEffectSound(12);
            SignalAndExecuteConfigurationAnimation(self->joystickRootVmId, 3);
            SignalSecondaryAnimationEvent(self->joystickRootVmId,
                static_cast<short>(selection->current + 7));
        }
        unsigned char *buttons = PollJoystickButtons();
        for (int button = 0; button < 31; ++button)
            if (buttons[button] & 0x80)
            {
                if (selection->current <= 4)
                    AssignMenuJoystickButton(self, selection->current, button);
                break;
            }
        if ((g_InputState.pressed & 0x102) && selection->current == 6)
        {
            LoadBindings(self);
            ApplyMenuJoystickBindings(self);
            goto Back;
        }
        if (g_InputState.pressed & 0x80001)
        {
            if (selection->current == 5)
            {
                LoadBindings(self);
                ApplyMenuJoystickBindings(self);
                QueueMeshEffectSound(10);
                return 1;
            }
            if (selection->current == 6)
            {
                g_JoystickBindings.fire = self->joystickBindings[0];
                g_JoystickBindings.bomb = self->joystickBindings[1];
                g_JoystickBindings.slow = self->joystickBindings[2];
                g_JoystickBindings.pause = self->joystickBindings[3];
                g_JoystickBindings.skip = self->joystickBindings[4];
                *reinterpret_cast<JoystickBindingsView *>(
                    reinterpret_cast<unsigned char *>(&g_MainRuntime) + 0x1cc) =
                    g_JoystickBindings;
            Back:
                QueueMeshEffectSound(11);
                SignalClose(self->joystickRootVmId, 6);
                SetConfigurationMenuState(&self->page, 4);
                return 1;
            }
        }
        break;
    }
    case 3: break;
    case 4:
        if (self->page.time.current >= 10)
        {
            SetConfigurationMenuPage(&self->page, 3);
            PopConfigurationSelection(&self->page.selection);
        }
        break;
    }
    return 1;
}
