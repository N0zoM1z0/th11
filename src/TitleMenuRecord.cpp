#include "TitleMenuRecord.hpp"
#include "MainRuntime.hpp"
#include <new>

struct ReplayCleanupState;
void __stdcall ClearReplayStageSegments(ReplayCleanupState *state, int stage);
extern TitleMenuRecordView *g_TitleReplayRecord;

namespace
{
struct ScopedReplayChainLock
{
    ScopedReplayChainLock()
    {
        if (g_MainRuntime.flags & 0x8000)
        {
            EnterCriticalSection(&g_MainRuntime.criticalSections[0]);
            ++g_MainRuntime.criticalDepth[0];
        }
    }
    ~ScopedReplayChainLock()
    {
        if (g_MainRuntime.flags & 0x8000)
        {
            LeaveCriticalSection(&g_MainRuntime.criticalSections[0]);
            --g_MainRuntime.criticalDepth[0];
        }
    }
};

__forceinline void ReleaseReplayChain(ChainElement *element)
{
    if (element)
    {
        ScopedReplayChainLock lock;
        UnregisterChainElement(element, g_ChainManager);
    }
}
}

// Native 0x435C50..0x435DFC, complete 429-byte owner. The eight stage-link
// destructors are real member lifetimes, not a dummy vector EH call.
TitleMenuRecordView::~TitleMenuRecordView()
{
    ::operator delete(entryData);
    for (int stage = 0; stage < 8; ++stage)
        ClearReplayStageSegments(reinterpret_cast<ReplayCleanupState *>(this), stage);

    ::operator delete(buffer018);
    buffer018 = 0;
    for (int i = 0; i < 8; ++i)
    {
        ::operator delete(buffers[i]);
        buffers[i] = 0;
    }

    ReleaseReplayChain(updateChain);
    ReleaseReplayChain(secondaryChain);
    ReleaseReplayChain(drawChain);
    if (g_TitleReplayRecord == this) g_TitleReplayRecord = 0;
    // C++ implicitly runs the eight ReplayRecordStageLink destructors here.
}

// The native callers explicitly invoke the destructor and then the scalar
// delete. Preserve their separate ownership decisions in the calling source.
void __stdcall DestroyTitleMenuRecord(TitleMenuRecordView *record)
{
    record->~TitleMenuRecordView();
}
