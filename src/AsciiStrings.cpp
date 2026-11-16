#include "AsciiManager.hpp"

// TH11 0x00401D50..0x00401DCB. Expire negative lifetimes and compact retained
// records in their original order. A lifetime of zero is removed on this tick.
void __stdcall AsciiManagerExpireStrings(AsciiManagerView *self)
{
    AsciiStringRecord *records = self->strings;
    int output = 0;
    int input = 0;
    if (input < self->stringCount)
    {
        AsciiStringRecord *source = records;
        AsciiStringRecord *destination = records;
        int *remaining = &records->remainingFrames;
        do
        {
            --*remaining;
            if (*remaining >= 0)
            {
                if (output != input)
                    *destination = *source;
                ++output;
                ++destination;
            }
            ++input;
            ++source;
            remaining += sizeof(AsciiStringRecord) / sizeof(int);
        } while (input < self->stringCount);
    }
    self->stringCount = output;
}
