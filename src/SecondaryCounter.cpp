#include "SecondaryCounter.hpp"

namespace
{
inline int Difference(int a, int b)
{
    return static_cast<int>(static_cast<unsigned int>(a) - b);
}
}

int SecondaryCounterView::Subtract(int amount)
{
    if (flags & 1)
    {
        scaled = Difference(scaled, amount);
        int base = static_cast<int>(static_cast<unsigned int>(baseline) * 7);
        value = static_cast<int>(static_cast<unsigned int>(Difference(scaled, base) / 7) + baseline);
    }
    else value = Difference(value, amount);
    return value;
}

void SecondaryCounterView::SetScaled(int amount)
{
    scaled = static_cast<int>(static_cast<unsigned int>(amount) * 7);
}

void SecondaryCounterView::SetFlag(unsigned int value)
{
    flags = (flags & ~1u) | (value & 1u);
}
