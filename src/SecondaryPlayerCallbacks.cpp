#include "SecondaryPlayerController.hpp"

int __fastcall UpdateSecondaryPlayerCallback(SecondaryDamageReferencesView *self)
{
    return UpdateSecondaryPlayerController(self);
}

int __fastcall DrawSecondaryPlayerCallback(SecondaryDamageReferencesView *self)
{
    return DrawSecondaryPlayerEmitters(self);
}
