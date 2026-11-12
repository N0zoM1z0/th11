#pragma once
#include "SecondaryActorState.hpp"

// Independently bounded 318-byte secondary host startup owner.
int __stdcall InitializeSecondaryHost(SecondaryMasterView *self, const char *scriptFilename);
