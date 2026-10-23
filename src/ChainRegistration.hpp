#pragma once
#include "Chain.hpp"

// The canonical /GL context naturally assigns element to ESI and priority to
// EBX. These declarations retain the ordinary source-level convention.
int __stdcall AddToUpdateChain(ChainElement *element, int priority);
int __stdcall AddToDrawChain(ChainElement *element, int priority);
