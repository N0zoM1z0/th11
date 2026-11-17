#include "SecondaryDebugHudLifetime.hpp"
#include <new>

// Keeps the original owning construction reachable through an ordinary
// dynamic C++ allocation. The source build is a local LTCG diagnostic.
extern "C" SecondaryDebugHudOwned *__stdcall SecondaryDebugHudConstructionProbe(int create)
{
    if (create)
        return new SecondaryDebugHudOwned;
    return 0;
}
