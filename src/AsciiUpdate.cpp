#include "AsciiManager.hpp"

// TH11 callback 0x004014A0..0x004014B5 receives the manager in ECX.
int __fastcall AsciiManagerOnUpdate(AsciiManagerView *self)
{
    AsciiManagerExpireStrings(self);
    ++self->frameCounter;
    return 1;
}
