#include "SecondaryProgramBrowser.hpp"
#include "ChainRegistration.hpp"
#include <new>

// Real registered priority-5 update and priority-44 draw callback owners.
int __fastcall SecondaryProgramBrowserUpdateCallback(SecondaryProgramBrowserView *self);
int __fastcall SecondaryProgramBrowserDrawCallback(SecondaryProgramBrowserView *self);

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
        link.next = link.previous = 0;
    }
};
__forceinline BrowserChainElement *MakeBrowserChain(
    ChainCallback callback, SecondaryProgramBrowserView *browser)
{
    BrowserChainElement *chain = new BrowserChainElement;
    chain->flags |= 3;
    chain->callback = callback;
    chain->unknown0c = 0;
    chain->auxiliaryCallback = 0;
    chain->context = browser;
    return chain;
}
}

// Full 0x438660..0x43870C native owner: register the two independent
// browser callback nodes without borrowing code from the adjacent destructor.
int __stdcall RegisterSecondaryProgramBrowserChains(SecondaryProgramBrowserView *browser)
{
    BrowserChainElement *update = MakeBrowserChain(
        reinterpret_cast<ChainCallback>(SecondaryProgramBrowserUpdateCallback), browser);
    AddToUpdateChain(update, 5);
    browser->updateChain = update;
    BrowserChainElement *draw = MakeBrowserChain(
        reinterpret_cast<ChainCallback>(SecondaryProgramBrowserDrawCallback), browser);
    AddToDrawChain(draw, 44);
    browser->drawChain = draw;
    return 0;
}
