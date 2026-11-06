#include "MenuJoystick.hpp"
#include "MeshEffects.hpp"

void __stdcall AssignMenuJoystickButton(MenuControllerView *self, int selection, int button)
{
    if (self->joystickBindings[selection] == button) return;
    if (selection != 0 && self->joystickBindings[0] == button)
        self->joystickBindings[0] = self->joystickBindings[selection];
    if (selection != 1 && self->joystickBindings[1] == button)
        self->joystickBindings[1] = self->joystickBindings[selection];
    if (selection != 2 && self->joystickBindings[2] == button)
        self->joystickBindings[2] = self->joystickBindings[selection];
    if (selection != 3 && self->joystickBindings[3] == button)
        self->joystickBindings[3] = self->joystickBindings[selection];
    if (selection != 4 && self->joystickBindings[4] == button)
        self->joystickBindings[4] = self->joystickBindings[selection];
    self->joystickBindings[selection] = static_cast<short>(button);
    ApplyMenuJoystickBindings(self);
    QueueMeshEffectSound(10);
}
