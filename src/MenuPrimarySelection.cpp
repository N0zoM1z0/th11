#include "MenuPrimary.hpp"

// Full native 0x40DF10..0x40DF30. The lower-bound mask maps naturally to
// the target's SETL/DEC/AND sequence while keeping the true upper clamp and
// the zero-count path independently observable.
int __fastcall SelectMenuItem(int value, SecondaryDebugSelectionView *selection)
{
    const int count = selection->count;
    if (count)
    {
        if (value >= count)
            return selection->current = count - 1;
        const int nonnegativeMask = value < 0 ? 0 : -1;
        return selection->current = value & nonnegativeMask;
    }
    return selection->current = value;
}
