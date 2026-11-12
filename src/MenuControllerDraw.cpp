#include "MenuController.hpp"

// Reviewed menu page draw boundaries. Page08 receives the controller in
// private EDI under the real /GL caller, whereas the natural source passes it
// explicitly and lets the compiler recover the register allocation.
int __stdcall DrawMenuPage08(MenuControllerView *self); // 0x43DAA0
int __stdcall DrawMenuPage0A(MenuControllerView *self); // 0x43F410
int __stdcall DrawMenuPage0B(MenuControllerView *self); // 0x43E2F0
void __stdcall DrawMenuPage0E(MenuControllerView *self); // 0x43FC50
void __stdcall DrawMenuPage0F(MenuControllerView *self); // 0x440520

// 0x439E80..0x439F0B: full five-route draw dispatcher, including the
// original eight-way 32-byte page switch table at 0x439EEC..0x439F0B.
// This is the draw partner to the 1224-byte UpdateMenuController owner.
int __fastcall DrawMenuController(MenuControllerView *self)
{
    switch (self->page.page)
    {
    case 8:
        DrawMenuPage08(self);
        return 1;
    case 10:
        DrawMenuPage0A(self);
        return 1;
    case 11:
        DrawMenuPage0B(self);
        return 1;
    case 14:
        DrawMenuPage0E(self);
        return 1;
    case 15:
        DrawMenuPage0F(self);
        return 1;
    }
    return 1;
}
