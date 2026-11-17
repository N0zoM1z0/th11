#include "SecondaryDebugHudLifetime.hpp"
#include "SecondaryHud.hpp"
#include "ChainRegistration.hpp"
#include "AnimationDocument.hpp"
#include "ErrorLog.hpp"
#include <new>

// Real game strings: the native file name is "front.anm" at 0x494108;
// the Shift-JIS resource error is stored at 0x493874. They remain
// external data objects, not duplicated second storage allocations.
extern const char g_SecondaryHudFrontAnimationFilename[];
extern const char g_SecondaryHudResourceErrorMessage[];

// The 336-byte visual initializer at 0x41A520 is independently owned.
// Keep the call intact until that complete native body is restored.
int __stdcall InitializeSecondaryDebugHudVisuals(SecondaryDebugHudOwned *owner);

// The update (41CFB0), front render (41CFC0) and HUD render (41CFD0)
// callbacks are three genuinely separate native functions.
int __fastcall SecondaryHudFrontDrawCallback(SecondaryHudView *owner);
int __fastcall SecondaryHudDrawCallback(SecondaryHudView *owner);

namespace
{
struct HudChainElement : ChainElement
{
    HudChainElement()
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

__forceinline HudChainElement *MakeHudChain(
    SecondaryDebugHudOwned *owner, ChainCallback callback)
{
    HudChainElement *chain = new HudChainElement;
    chain->SetCallback(callback);
    chain->context = owner;
    chain->flags = (chain->flags & ~2u) | 1u;
    return chain;
}
}

// Complete 339-byte original 0x41A3C0..0x41A512 registration owner.
// Native private register allocation and the font/document error strings
// still need a real caller context; the source retains original behavior.
int __stdcall RegisterSecondaryHudChains(SecondaryDebugHudOwned *owner)
{
    AnimationDocumentView *document =
        AcquireAnimationDocument(5, g_SecondaryHudFrontAnimationFilename);
    owner->ownerMarker = reinterpret_cast<int>(document);
    if (!document)
    {
        AppendLog(&g_RuntimeLog.buffer, g_SecondaryHudResourceErrorMessage);
        return -1;
    }

    if (InitializeSecondaryDebugHudVisuals(owner))
        return -1;

    HudChainElement *update = MakeHudChain(
        owner, reinterpret_cast<ChainCallback>(SecondaryHudUpdateCallback));
    AddToUpdateChain(update, 24);
    owner->updateChain = update;

    HudChainElement *draw = MakeHudChain(
        owner, reinterpret_cast<ChainCallback>(SecondaryHudFrontDrawCallback));
    AddToDrawChain(draw, 57);
    owner->drawChain = draw;

    HudChainElement *overlay = MakeHudChain(
        owner, reinterpret_cast<ChainCallback>(SecondaryHudDrawCallback));
    AddToDrawChain(overlay, 42);
    owner->overlayChain = overlay;
    return 0;
}
