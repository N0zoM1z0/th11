#include "TitleMenuRecord.hpp"
#include "MainRuntime.hpp"
#include "SecondaryReplayState.hpp"

struct ReplayCleanupState;
void __stdcall ClearReplayStageSegments(ReplayCleanupState *state, int stage);

namespace
{
inline void RemoveTitleRecordChain(ChainElement *element)
{
    if (!element)
        return;
    ChainManagerView *manager = g_ChainManager;
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

// 00435C50. Callers invoke the destructor directly (LTCG passes this on the
// stack); the eight stage links are destroyed by the member teardown.
TitleMenuRecordView::~TitleMenuRecordView()
{
    delete static_cast<unsigned char *>(entryData);
    for (int i = 0; i < 8; ++i)
        ClearReplayStageSegments(reinterpret_cast<ReplayCleanupState *>(this), i);
    delete static_cast<unsigned char *>(buffer018);
    buffer018 = 0;
    for (int i = 0; i < 8; ++i)
    {
        delete static_cast<unsigned char *>(buffers[i]);
        buffers[i] = 0;
    }
    RemoveTitleRecordChain(updateChain);
    RemoveTitleRecordChain(secondaryChain);
    RemoveTitleRecordChain(drawChain);
    if (reinterpret_cast<ReplayStateView *>(this) == g_SecondaryReplayCurrent)
        g_SecondaryReplayCurrent = 0;
}
