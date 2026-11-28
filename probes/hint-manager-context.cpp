#include "Hint.hpp"
#include "ChainRegistration.hpp"
#include <new>

// Diagnostic only: construct and initialize the manager as the create function
// does, free its lists and call the chain helpers with generic values.
extern "C" int __stdcall HintManagerProbe(ChainElement *external, int priority, void *storage)
{
    HintInf *self = new (storage) HintInf;
    int result = InitializeHintManager(self);
    FreeHintLists(self);
    if (external)
        result += AddToUpdateChain(external, priority);
    return result + AddToDrawChain(external, priority);
}
