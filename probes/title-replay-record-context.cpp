#include "TitleMenuRecord.hpp"

void __stdcall DestroyTitleMenuRecord(TitleMenuRecordView *record);
extern "C" void __stdcall TitleReplayRecordProbe(TitleMenuRecordView *record)
{
    DestroyTitleMenuRecord(record);
}
