#pragma once

#include "MenuController.hpp"
#include "InputBackend.hpp"

void __stdcall ApplyMenuJoystickBindings(MenuControllerView *self);
void __stdcall AssignMenuJoystickButton(MenuControllerView *self, int selection, int button);
