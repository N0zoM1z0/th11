#include "SecondaryHostLookup.hpp"
#include "SecondaryHud.hpp"
#include "SecondaryDialogue.hpp"
#include "SecondaryClearing.hpp"

// The 0x410720 native owner reads the current dialogue's frame delay through
// the HUD, without testing either pointer for null.
int __cdecl ReadSecondaryHostDialogueDelay()
{
    SecondaryHudView *hud = reinterpret_cast<SecondaryHudView *>(g_SecondaryActorHud);
    return hud->dialogue->delay;
}

// This overlay deliberately does not assume an original class identity: the
// observed cleanup chain has its next link at +8 and its identity at +0x6c.
struct SecondaryClearIdentityNode
{
    void *vtable;
    unsigned int flags;
    SecondaryClearIdentityNode *next;
    int state;
    unsigned char unknown010[0x6c - 0x10];
    int identity;
};
typedef char SecondaryClearIdentityNextOffset[
    (offsetof(SecondaryClearIdentityNode, next) == 8) ? 1 : -1];
typedef char SecondaryClearIdentityFieldOffset[
    (offsetof(SecondaryClearIdentityNode, identity) == 0x6c) ? 1 : -1];

// Complete native 0x4108D0..0x4108EE: zero is not a searchable identity.
SecondaryClearIdentityNode *__fastcall FindSecondaryClearObjectByIdentity(int identity)
{
    SecondaryClearIdentityNode *node = reinterpret_cast<SecondaryClearIdentityNode *>(
        g_SecondaryClearManager->first);
    if (identity != 0)
        for (; node; node = node->next)
            if (node->identity == identity) return node;
    return 0;
}
