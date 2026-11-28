#include "Hint.hpp"
#include "ChainRegistration.hpp"
#include "MainRuntime.hpp"
#include <new>
#include <string.h>

namespace
{
struct HintChainElement : ChainElement
{
    HintChainElement()
    {
        flags &= ~1u;
        callback = 0;
        unknown0c = 0;
        auxiliaryCallback = 0;
        priority = 0;
        link.element = this;
        link.next = 0;
        link.previous = 0;
    }
};

__forceinline unsigned char HintMode()
{
    return *(reinterpret_cast<const unsigned char *>(&g_MainRuntime) + 0x1ea);
}
}

// 0x420E90
HintInf::HintInf()
{
    memset(this, 0, sizeof(HintInf));
    flags |= 2u;
    // Native writes all sixteen lists as straight-line code; VC9 does not unroll
    // the equivalent loop.
    ResetList(lists[0]); ResetList(lists[1]); ResetList(lists[2]); ResetList(lists[3]);
    ResetList(lists[4]); ResetList(lists[5]); ResetList(lists[6]); ResetList(lists[7]);
    ResetList(lists[8]); ResetList(lists[9]); ResetList(lists[10]); ResetList(lists[11]);
    ResetList(lists[12]); ResetList(lists[13]); ResetList(lists[14]); ResetList(lists[15]);
    g_HintManager = this;
}

// 0x420F90: update priority 25, draw priority 51; hint mode 1 or 2 loads both
// hint files.
int __stdcall InitializeHintManager(HintInf *self)
{
    HintChainElement *update = new HintChainElement;
    update->callback = reinterpret_cast<ChainCallback>(HintManagerUpdateCallback);
    update->unknown0c = 0;
    update->auxiliaryCallback = 0;
    update->context = self;
    update->flags = (update->flags & ~2u) | 1u;
    AddToUpdateChain(update, 25);
    self->updateElement = update;
    HintChainElement *draw = new HintChainElement;
    draw->unknown0c = 0;
    draw->auxiliaryCallback = 0;
    draw->callback = reinterpret_cast<ChainCallback>(HintManagerDrawCallback);
    draw->context = self;
    draw->flags = (draw->flags & ~2u) | 1u;
    AddToDrawChain(draw, 51);
    self->drawElement = draw;
    self->unknown010 = 0;
    if (HintMode())
    {
        LoadHints(self, "hint/hint_auto.txt", 0);
        LoadHints(self, "hint/hint_user.txt", 1);
    }
    return 0;
}

// 0x421210: free the data of every node in all sixteen lists.
void __stdcall FreeHintLists(HintInf *self)
{
    for (int i = 0; i < 8; ++i)
    {
        for (HintNode *node = self->lists[i].first; node; )
        {
            HintNode *next = node->next;
            operator delete(node->data);
            node = next;
        }
        for (HintNode *node = self->lists[i + 8].first; node; )
        {
            HintNode *next = node->next;
            operator delete(node->data);
            node = next;
        }
    }
}
