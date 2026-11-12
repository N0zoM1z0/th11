#include "MenuController.hpp"

// Source-open native page drawing boundaries. The three no-argument page
// draws use the controller's private optimized register context; pages E/F
// take the menu object on the stack and pop that one argument themselves.
void __cdecl DrawMenuPage08(); // 0x43DAA0
void __cdecl DrawMenuPage0A(); // 0x43F410
void __cdecl DrawMenuPage0B(); // 0x43E2F0
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
        DrawMenuPage08();
        return 1;
    case 10:
        DrawMenuPage0A();
        return 1;
    case 11:
        DrawMenuPage0B();
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
