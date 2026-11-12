#include "../src/SecondaryActorState.hpp"
extern void __stdcall DestroySecondaryMaster(SecondaryMasterView *master);
extern "C" void __stdcall SecondaryMasterDestructionProbe(SecondaryMasterView *master)
{
    DestroySecondaryMaster(master);
}
