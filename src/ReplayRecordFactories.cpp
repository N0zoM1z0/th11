#include "ReplayRecordFactories.hpp"
#include <new>
#include <string.h>

struct ReplayInitStateView;
int __stdcall LoadReplayStageData(ReplayInitStateView *state, void *source);

namespace
{
// The native record is a full 0x2DC-byte class; its default constructor
// initializes eight stage nodes before clearing the complete record.
__forceinline TitleMenuRecordView *AllocateReplayRecord()
{
    return new TitleMenuRecordView;
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
