#include "SecondaryPlayerWrapItems.hpp"

extern "C" int __stdcall SecondaryPlayerWrapItemsProbe(SecondaryDamageReferencesView *self)
{
    int result = UpdateSecondaryPlayerMovement(self);
    AttractSecondaryPlayerItems();
    return result;
}
