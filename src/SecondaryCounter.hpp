#pragma once
#include <stddef.h>

// Observed prefix only; the full enclosing object and original type names remain open.
struct SecondaryCounterView
{
    int Subtract(int amount);
    void SetScaled(int amount);
    void SetFlag(unsigned int value);
    int value, unknown004, unknown008, scaled, baseline;
    unsigned int flags;
};

typedef char SecondaryCounterScaledOffset[(offsetof(SecondaryCounterView, scaled) == 0xc) ? 1 : -1];
typedef char SecondaryCounterFlagOffset[(offsetof(SecondaryCounterView, flags) == 0x14) ? 1 : -1];
