#include "Item.hpp"
#include "ChainRegistration.hpp"

// Diagnostic only: create and delete the manager the way the game does and
// call the chain helpers with generic values.
extern "C" int __stdcall ItemManagerProbe(ChainElement *external, int priority)
{
    int result = CreateItemManager() != 0;
    if (external)
        result += AddToUpdateChain(external, priority);
    result += AddToDrawChain(external, priority);
    DeleteItemManager();
    return result;
}
