// Independent input; this diagnostic image is never executed.
#include "SecondaryActorState.hpp"
#include <new>
extern "C" int __stdcall SecondaryActorLifetimeProbe(void *storage, const char *name,
                                                     SecondaryActorManagerView *actor)
{
    new (storage) SecondaryActorManagerView(name);
    delete actor;
    return SecondaryActorReferenceStatus();
}
