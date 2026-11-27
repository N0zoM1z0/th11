#pragma once

#include "AnimationVmId.hpp"

// The renderer's next-VM-ID counter is itself an ID object. Native
// publishers increment it, re-read it for the zero test and increment it
// again on wrap; a plain unsigned counter lets VC9 fold that second step
// into a constant store.
inline AnimationVmId &operator++(AnimationVmId &id)
{
    ++id.value;
    return id;
}

inline int operator==(const AnimationVmId &id, unsigned int value)
{
    return id.value == value;
}
