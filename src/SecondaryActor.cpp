#include "SecondaryActor.hpp"

int __stdcall UpdateSecondaryActor(SecondaryManager *self)
{
    return UpdateSecondaryActorState(reinterpret_cast<SecondaryActorStateView *>(
        reinterpret_cast<unsigned char *>(self) + sizeof(SecondaryManager)));
}
