#include "SecondaryDialogue.hpp"
#include <stddef.h>

// Observed state indirection used by 0042F540. Names describe physical
// storage, not the original runtime's type identity.
struct SecondaryDialogueStateFlagView
{
    unsigned char unknown000[4];
    unsigned int flags;
};
struct SecondaryDialogueRuntimeFlagView
{
    unsigned char unknown000[8];
    SecondaryDialogueStateFlagView *state;
};
typedef char DialogueStateFlagsOffset[(offsetof(SecondaryDialogueStateFlagView, flags) == 4) ? 1 : -1];
typedef char DialogueRuntimeStateOffset[(offsetof(SecondaryDialogueRuntimeFlagView, state) == 8) ? 1 : -1];

// Complete original owner 0042F540..0042F55A; caller is controller 0042BF30.
int __cdecl IsSecondaryDialogueStateFlag2Set()
{
    SecondaryDialogueRuntimeFlagView *session =
        reinterpret_cast<SecondaryDialogueRuntimeFlagView *>(g_SecondaryDialogueSession);
    SecondaryDialogueStateFlagView *state = session->state;
    if (state && (state->flags & 2))
        return 1;
    return 0;
}
