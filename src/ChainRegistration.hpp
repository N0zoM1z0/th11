#pragma once
#include "Chain.hpp"

// Conventional source views; target entries receive element in ESI and
// priority in EBX. Their linked calling context remains unresolved.
int __stdcall AddToUpdateChain(ChainElement *element, int priority);
int __stdcall AddToDrawChain(ChainElement *element, int priority);
