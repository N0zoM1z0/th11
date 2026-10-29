// Independent object input; this diagnostic image is never executed.
#include "SecondaryActor.hpp"
extern "C" int __stdcall SecondaryActorProbe(SecondaryManager *self)
{
    return UpdateSecondaryActor(self);
}
