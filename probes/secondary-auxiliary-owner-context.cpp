#include "SecondaryAuxiliaryOwner.hpp"
#include "ChainRegistration.hpp"

// Independent caller binds the native allocation/owner and ensures the
// registration priorities retain a genuinely dynamic use in this LTCG unit.
extern "C" SecondaryAuxiliaryOwnerView *__cdecl AuxiliaryOwnerProbe(
    ChainElement *other, int priority)
{
    SecondaryAuxiliaryOwnerView *self = CreateSecondaryAuxiliaryOwner();
    if (other)
    {
        AddToUpdateChain(other, priority);
        AddToDrawChain(other, priority);
    }
    return self;
}
