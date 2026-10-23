// Non-runnable caller context. Inputs remain independent and unknown.
#include "../src/Random.hpp"

extern "C" unsigned int __stdcall RandomProbe(RandomStateView *a,
                                              RandomStateView *b, int wide)
{
    if (wide)
    {
        unsigned int first = RandomNext32(a);
        return first ^ RandomNext32(b);
    }
    unsigned int first = RandomNext16(a);
    return first ^ RandomNext16(b);
}
