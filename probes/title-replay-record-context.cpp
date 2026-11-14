#include "TitleMenuRecord.hpp"
#include <new>

// Non-runnable link context: release a replay record as its owners do.
extern "C" void __stdcall TitleReplayRecordProbe(TitleMenuRecordView *record)
{
    record->~TitleMenuRecordView();
    ::operator delete(record);
}
