#include "ReplayFileBrowserLifetime.hpp"
#include "ChainRegistration.hpp"
#include "AnimationVm.hpp"
#include "AnimationSpriteBinding.hpp"
#include "AsciiLifecycle.hpp"
#include <new>

// The native source publishes the two callback addresses 0x438420/0x438430;
// their complete separate owner functions remain to be established.
int __fastcall ReplayFileBrowserUpdateCallback(ReplayFileBrowserView *self);
int __fastcall ReplayFileBrowserDrawCallback(ReplayFileBrowserView *self);

namespace
{
struct BrowserChainElement : ChainElement
{
    __forceinline BrowserChainElement()
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

__forceinline BrowserChainElement *MakeReplayChain(
    ChainCallback callback, ReplayFileBrowserView *browser)
{
    BrowserChainElement *chain = new BrowserChainElement;
    chain->callback = callback;
    chain->unknown0c = 0;
    chain->auxiliaryCallback = 0;
    chain->context = browser;
    chain->flags = (chain->flags & ~2u) | 1u;
    return chain;
}
}

// Native 0x437D50..0x437E35 (230-byte original). Register priorities are
// update 15 and draw 41; the two separately observed callback pointers
// are associated with the full browser owner, not the leaderboard manager.
int __stdcall InitializeReplayFileBrowser(ReplayFileBrowserView *self)
{
    self->animation = g_AsciiManager->animation;
    BrowserChainElement *update = MakeReplayChain(
        reinterpret_cast<ChainCallback>(ReplayFileBrowserUpdateCallback), self);
    AddToUpdateChain(update, 15);
    self->update = update;

    BrowserChainElement *draw = MakeReplayChain(
        reinterpret_cast<ChainCallback>(ReplayFileBrowserDrawCallback), self);
    AddToDrawChain(draw, 41);
    self->draw = draw;

    AnimationVmView *vm = reinterpret_cast<AnimationVmView *>(
        reinterpret_cast<unsigned char *>(self) + 0x18);
    ResetAnimationVm(vm);
    *reinterpret_cast<AnimationDocumentView **>(
        reinterpret_cast<unsigned char *>(self) + 0x3c8) = self->animation;
    BindAnimationSprite(self->animation, 0xc4, vm);
    return 0;
}
