#pragma once
#include "SecondaryScript.hpp"

struct SecondaryActorStateView;
// Reviewed full native state update; its implementation remains source-open.
int __stdcall UpdateSecondaryActorState(SecondaryActorStateView *state);
int __stdcall UpdateSecondaryActor(SecondaryManager *self);
