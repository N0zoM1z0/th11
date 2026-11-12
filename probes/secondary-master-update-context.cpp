#include "../src/SecondaryActorState.hpp"
extern int __stdcall UpdateSecondaryMasterActors(SecondaryMasterView *master);
extern "C" int __stdcall SecondaryMasterUpdateProbe(SecondaryMasterView *master)
{
    return UpdateSecondaryMasterActors(master);
}
