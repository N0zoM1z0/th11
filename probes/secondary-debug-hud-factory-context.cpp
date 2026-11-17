#include "SecondaryDebugHudLifetime.hpp"

extern "C" SecondaryDebugHudOwned *__stdcall SecondaryDebugHudFactoryProbe(int create)
{
    if (create)
        return CreateSecondaryDebugHud();
    return 0;
}
