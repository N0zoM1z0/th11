// Non-runnable link context: the menu controller update and draw reach every
// menu page and its support helpers.
#include "../src/MenuController.hpp"
int __fastcall DrawMenuController(MenuControllerView *self);
extern "C" int __stdcall MenuAllProbe(MenuControllerView *self)
{
    return UpdateMenuController(self) + DrawMenuController(self);
}
