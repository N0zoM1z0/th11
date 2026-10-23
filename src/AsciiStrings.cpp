#include "AsciiManager.hpp"

// TH11 0x00401D50..0x00401DCB. Expire negative lifetimes and compact retained
// records in their original order. A lifetime of zero is removed on this tick.
void __stdcall AsciiManagerExpireStrings(AsciiManagerView *self)
{
    int output = 0;
    for (int input = 0; input < self->stringCount; ++input)
    {
        --self->strings[input].remainingFrames;
        if (self->strings[input].remainingFrames >= 0)
        {
            if (output != input)
                self->strings[output] = self->strings[input];
            ++output;
        }
    }
    self->stringCount = output;
}
