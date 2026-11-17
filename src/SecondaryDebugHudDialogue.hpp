#pragma once

#include "SecondaryActorRuntime.hpp"
#include <stddef.h>

// Only the eight observed animation IDs are named. The original dialogue
// class and fields preceding +0x40 remain unknown.
struct SecondaryHudDialogueOwnedView
{
    unsigned char unknown000[0x40];
    unsigned int vmIds[8];
};

typedef char SecondaryHudDialogueIdsOffset[
    (offsetof(SecondaryHudDialogueOwnedView, vmIds) == 0x40) ? 1 : -1];

void __stdcall DestroySecondaryHudDialogue(SecondaryHudDialogueOwnedView *self);
