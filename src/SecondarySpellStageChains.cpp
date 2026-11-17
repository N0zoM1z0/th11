#include "SecondarySpellStageOwned.hpp"
#include <new>

namespace
{
// Natural C++ representation of the target's 0x24-byte chain node
// constructor: only the fields touched by the native are initialized.
struct SpellStageChainElement : ChainElement
{
    SpellStageChainElement()
    {
        flags &= ~1u;
        priority = 0;
        callback = 0;
        unknown0c = 0;
        auxiliaryCallback = 0;
        link.element = this;
        link.next = 0;
        link.previous = 0;
    }
};

__forceinline SpellStageChainElement *MakeSpellChain(
    SecondarySpellStageOwned *self, ChainCallback callback)
{
    SpellStageChainElement *element = new SpellStageChainElement;
    element->SetCallback(callback);
    element->context = self;
    element->flags = (element->flags & ~2u) | 1u;
    return element;
}
}

// The two native callbacks are separate small code owners:
// 0x40C630 calls the 624-byte update owner; 0x40C640 tail-dispatches
// the 370-byte stage-spell HUD owner. Never replace either with an ASM JMP.
int __fastcall SecondarySpellUpdateCallback(void *context)
{
    return UpdateSecondarySpellStageState(
        reinterpret_cast<SecondarySpellStageView *>(context));
}

int __fastcall SecondarySpellDrawCallback(void *context)
{
    return DrawSecondarySpellStage(
        reinterpret_cast<SecondarySpellStageView *>(context));
}

// Native 0x40BD00..0x40BDFF: two independently allocated chain records,
// priorities 22 and 9, followed by an ordinary timer frame -1 reset.
int __stdcall RegisterSecondarySpellStageChains(SecondarySpellStageOwned *self)
{
    SpellStageChainElement *update =
        MakeSpellChain(self, SecondarySpellUpdateCallback);
    AddToUpdateChain(update, 22);
    self->updateChain = update;

    SpellStageChainElement *draw =
        MakeSpellChain(self, SecondarySpellDrawCallback);
    AddToDrawChain(draw, 9);
    self->drawChain = draw;

    self->time.SetFrame(-1);
    return 0;
}
