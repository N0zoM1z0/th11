#include "ReplayRecordFactories.hpp"
#include "SecondaryReplayState.hpp"
#include <new>
#include <string.h>

int __stdcall LoadReplayStageData(ReplayInitStateView *state, void *source);

namespace
{
// The full 0x2DC-byte storage is physically the same replay record base.
// Placing its wipe in the final derived constructor body restores the native
// eight-member EH vector construction followed immediately by memset.
struct ReplayRecordCompleteStorage : TitleMenuRecordView
{
    ReplayRecordCompleteStorage() { memset(this, 0, sizeof(*this)); }
};
typedef char ReplayRecordCompleteSize[
    (sizeof(ReplayRecordCompleteStorage) == 0x2dc) ? 1 : -1];
__forceinline TitleMenuRecordView *AllocateReplayRecord()
{
    return new ReplayRecordCompleteStorage;
}
}

// Complete native 0x435E00..0x435EC6: create replay storage, call the real
// recording registry and unwind/delete the allocation on initialization error.
TitleMenuRecordView *__stdcall CreateReplayRecording(int mode)
{
    TitleMenuRecordView *record = AllocateReplayRecord();
    if (g_ReplayRecordRegistry.InitializeRecord(record, mode) != 0)
    {
        if (record)
        {
            record->~TitleMenuRecordView();
            ::operator delete(record);
        }
        return 0;
    }
    return record;
}

// Complete native 0x435EF0..0x435FB5: a second distinct 0x2DC-byte owner
// sets mode 2 and loads its stage data, then releases both C++ and raw storage
// on failure. The original does not test record for null after construction.
TitleMenuRecordView *__stdcall CreateReplayPlayback(const char *filename)
{
    TitleMenuRecordView *record = AllocateReplayRecord();
    *reinterpret_cast<unsigned int *>(
        reinterpret_cast<unsigned char *>(record) + 0x10) = 2;
    if (LoadReplayStageData(reinterpret_cast<ReplayInitStateView *>(record),
            const_cast<char *>(filename)) != 0)
    {
        record->~TitleMenuRecordView();
        ::operator delete(record);
        return 0;
    }
    return record;
}
