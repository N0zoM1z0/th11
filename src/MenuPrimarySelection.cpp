#include "MenuPrimary.hpp"

int __fastcall SelectMenuItem(int value, SecondaryDebugSelectionView *selection)
{
    if (selection->count)
        return selection->current = value >= selection->count ?
            static_cast<int>(static_cast<unsigned int>(selection->count) - 1) :
            value < 0 ? 0 : value;
    return selection->current = value;
}
