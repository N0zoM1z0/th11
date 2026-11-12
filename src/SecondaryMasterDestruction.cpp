#include "SecondaryActorState.hpp"
#include "SecondaryGameplay.hpp"
#include "Chain.hpp"
#include "AnimationDocument.hpp"
#include "StageResource.hpp"
#include "MainRuntime.hpp"
#include <stdlib.h>
#include <new>

// Original 0x40D0C0 is the independently exact, source-present 201-byte
// precleanup dependency. Both call sites share the real master object;
// its native private EAX calling convention remains compiler-owned.
void __stdcall PrepareSecondaryMasterDestruction(SecondaryMasterView *self);
void __stdcall DestroyAnimationDocument(AnimationDocumentView *file);

namespace
{
__forceinline ChainElement *MasterChain(SecondaryMasterView *self, int offset)
{
    return *reinterpret_cast<ChainElement **>(
        reinterpret_cast<unsigned char *>(self) + offset);
}

__forceinline void UnregisterMasterChain(ChainElement *element)
{
    ChainManagerView *manager = g_ChainManager;
    if (!element) return;
    if (g_MainRuntime.flags & 0x8000)
    {
        EnterCriticalSection(&g_MainRuntime.criticalSections[0]);
        ++g_MainRuntime.criticalDepth[0];
    }
    UnregisterChainElement(element, manager);
    if (g_MainRuntime.flags & 0x8000)
    {
        LeaveCriticalSection(&g_MainRuntime.criticalSections[0]);
        --g_MainRuntime.criticalDepth[0];
    }
}
}

// Complete native 0x410E90..0x411005 (374 bytes). There is no invented
// null-program guard before the original unconditional 32-file loop.
void __stdcall DestroySecondaryMaster(SecondaryMasterView *self)
{
    PrepareSecondaryMasterDestruction(self);
    UnregisterMasterChain(MasterChain(self, 8));
    UnregisterMasterChain(MasterChain(self, 12));

    for (int offset = 0x0c; offset < 0x8c; offset += 4)
    {
        SecondaryProgramView *program = self->program;
        void *entry = *reinterpret_cast<void **>(reinterpret_cast<char *>(program) + offset);
        if (entry) free(entry);
    }
    SecondaryProgramView *program = self->program;
    if (program) delete program;
    self->program = 0;

    if (!(g_StageResourceFlags & 9))
    {
        int index = 8;
        int offset = 0x4350e0;
        int remaining = 4;
        do
        {
            if (index >= 0 && index < 32)
            {
                unsigned char *base = reinterpret_cast<unsigned char *>(g_RenderBatch);
                AnimationDocumentView **slot = reinterpret_cast<AnimationDocumentView **>(base + offset);
                AnimationDocumentView *file = *slot;
                if (file)
                {
                    DestroyAnimationDocument(file);
                    ::operator delete(*slot);
                    *slot = 0;
                }
            }
            offset += 4;
            ++index;
            --remaining;
        } while (remaining);
    }
    g_SecondaryMaster = 0;
}
