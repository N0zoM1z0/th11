#pragma once
#include "SecondaryScript.hpp"

struct SecondaryActorStateView;
// Complete state implementation is in SecondaryActorUpdate.cpp; codegen remains open.
int __stdcall UpdateSecondaryActorState(SecondaryActorStateView *state);
int __stdcall UpdateSecondaryActor(SecondaryManager *self);
