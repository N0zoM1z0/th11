#include "SecondaryAuxiliaryOwner.hpp"
#include "ChainRegistration.hpp"
#include "MainRuntime.hpp"
#include "ErrorLog.hpp"
#include <new>

extern const char g_SecondaryAuxiliaryAnimationName[];
extern const char g_SecondaryAuxiliaryAnimationError[];
int __fastcall SecondaryAuxiliaryUpdateCallback(SecondaryAuxiliaryOwnerView *self);
int __fastcall SecondaryAuxiliaryDrawCallback(SecondaryAuxiliaryOwnerView *self);

namespace
{
struct AuxiliaryChainNode : ChainElement
{
    AuxiliaryChainNode()
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

__forceinline AuxiliaryChainNode *RegisterAuxiliaryChain(
    SecondaryAuxiliaryOwnerView *self, ChainCallback callback, int priority, bool draw)
{
    AuxiliaryChainNode *chain = new AuxiliaryChainNode;
    chain->callback = callback;
    chain->unknown0c = 0;
    chain->auxiliaryCallback = 0;
    chain->context = self;
    chain->flags = (chain->flags & ~2u) | 1u;
    if (draw) AddToDrawChain(chain, priority);
    else AddToUpdateChain(chain, priority);
    return chain;
}

__forceinline void UnregisterAuxiliaryChain(ChainElement *element)
{
    ChainManagerView *manager = g_ChainManager;
    if (!element) return;
    if (g_MainRuntime.flags & 0x8000)
    {
        EnterCriticalSection(&g_MainRuntime.criticalSections[0]);
        ++g_MainRuntime.criticalDepth[0];
    }
    UnregisterChainElement(manager, element);
    if (g_MainRuntime.flags & 0x8000)
    {
        LeaveCriticalSection(&g_MainRuntime.criticalSections[0]);
        --g_MainRuntime.criticalDepth[0];
    }
}
}

// 0x40E0A0..0x40E18D, 238 bytes, original two chain registrations, loaded
// animation slot 6 and independent logging failure return.
int __stdcall InitializeSecondaryAuxiliaryOwner(SecondaryAuxiliaryOwnerView *self)
{
    self->document = AcquireAnimationDocument(6, g_SecondaryAuxiliaryAnimationName);
    if (!self->document)
    {
        AppendLog(&g_RuntimeLog.buffer, g_SecondaryAuxiliaryAnimationError);
        return -1;
    }
    self->updateChain = RegisterAuxiliaryChain(self,
        reinterpret_cast<ChainCallback>(SecondaryAuxiliaryUpdateCallback), 23, false);
    self->drawChain = RegisterAuxiliaryChain(self,
        reinterpret_cast<ChainCallback>(SecondaryAuxiliaryDrawCallback), 31, true);
    return 0;
}

// 0x40E190..0x40E23F, 176 bytes, two independent chain-removal locks and
// global clear. It does not delete the retained animation document.
void __fastcall DestroySecondaryAuxiliaryOwner(SecondaryAuxiliaryOwnerView *self)
{
    UnregisterAuxiliaryChain(self->updateChain);
    UnregisterAuxiliaryChain(self->drawChain);
    g_SecondaryAuxiliaryOwner = 0;
}

// 0x40E240..0x40E29E, 95 bytes. Allocation failure follows the native
// null-record path; initialization failure tears down the real large owner.
SecondaryAuxiliaryOwnerView *__cdecl CreateSecondaryAuxiliaryOwner()
{
    SecondaryAuxiliaryOwnerView *self = new SecondaryAuxiliaryOwnerView;
    if (self)
    {
        self->flags |= 2u;
        g_SecondaryAuxiliaryOwner = self;
    }
    if (InitializeSecondaryAuxiliaryOwner(self))
    {
        if (self)
        {
            DestroySecondaryAuxiliaryOwner(self);
            ::operator delete(self);
        }
        return 0;
    }
    return self;
}
