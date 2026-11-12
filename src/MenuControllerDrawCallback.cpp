#include "MenuController.hpp"

int __fastcall DrawMenuController(MenuControllerView *self);

// Original TH11 callback at 0x439F20..0x439F26: registered by the 275-byte
// title resource owner and forwarding to the full menu draw dispatcher.
int __fastcall MenuControllerDrawCallback(MenuControllerView *self)
{
    return DrawMenuController(self);
}
