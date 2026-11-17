#include "SecondaryDebugHudLifetime.hpp"
#include "SecondaryActorRuntime.hpp"
#include <string.h>

// Complete 0x41A2F0 native constructor: the 9/2/4 animation VM arrays,
// the one standalone VM, and the two flag subobjects are constructed as
// actual C++ members before the game's full 0x4458-byte owner reset.
SecondaryDebugHudOwned::SecondaryDebugHudOwned()
{
    memset(this, 0, sizeof(*this));
    flags |= 2u;
    g_SecondaryActorHud = reinterpret_cast<SecondaryActorHudView *>(this);
}
