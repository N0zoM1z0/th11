// Independent record and amount. The diagnostic image is never executed.
#include "SecondaryCounter.hpp"
extern "C" int __stdcall SecondaryCounterProbe(int kind, SecondaryCounterView *counter, int amount)
{
    if (kind == 0) return counter->Subtract(amount);
    if (kind == 1) counter->SetScaled(amount);
    else counter->SetFlag(amount);
    return counter->value;
}
