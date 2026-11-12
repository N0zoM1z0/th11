#include "MenuController.hpp"
int __fastcall DrawMenuController(MenuControllerView *menu);
extern "C" int __stdcall MenuControllerDrawProbe(MenuControllerView *menu)
{
    return DrawMenuController(menu);
}
