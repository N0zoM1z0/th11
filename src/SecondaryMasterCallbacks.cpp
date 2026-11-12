#include "SecondaryActorState.hpp"

// Native 0x4111A0..0x4111A6: update-chain adapter for the complete 105-byte
// master actor tick. The stdcall callee cleans its one stack argument.
int __stdcall UpdateSecondaryMasterActors(SecondaryMasterView *self);
int __fastcall SecondaryMasterUpdateCallback(void *context)
{
    return UpdateSecondaryMasterActors(static_cast<SecondaryMasterView *>(context));
}

// Native 0x4111B0..0x4111B5: the draw-chain acknowledgement callback.
int __fastcall SecondaryMasterDrawCallback(void *context)
{
    return 1;
}
