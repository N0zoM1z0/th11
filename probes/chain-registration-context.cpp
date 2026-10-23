// Non-runnable compiler context: retain independent registration calls with
// unknown elements/priorities, allowing VC9 to choose its internal convention.
#include "../src/ChainRegistration.hpp"

extern "C" int __stdcall ChainRegistrationProbe(ChainElement *a, int priorityA,
                                                ChainElement *b, int priorityB, int draw)
{
    if (draw)
    {
        int result = AddToDrawChain(a, priorityA);
        return result + AddToDrawChain(b, priorityB);
    }
    int result = AddToUpdateChain(a, priorityA);
    return result + AddToUpdateChain(b, priorityB);
}
