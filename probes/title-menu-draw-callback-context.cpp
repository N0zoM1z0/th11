#include "MenuController.hpp"
int __fastcall MenuControllerDrawCallback(MenuControllerView *self);
extern "C" int __stdcall TitleMenuDrawCallbackProbe(MenuControllerView *self)
{
    return MenuControllerDrawCallback(self);
}
