// Independent object input; this diagnostic image is never executed.
#include "SecondaryActorState.hpp"
extern "C" void __stdcall SecondaryActorThreadsProbe(SecondaryManager *self)
{
    ReleaseSecondaryAuxiliaryThreads(self);
    ResetSecondaryMainThread(self);
}
