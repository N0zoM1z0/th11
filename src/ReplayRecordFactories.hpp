#pragma once
#include "TitleMenuRecord.hpp"

// Native physical 0x2DC-byte allocation, eight embedded stage-link members
// at +0xA4, and external registration/loader owners.
struct ReplayRecordRegistryView
{
    int InitializeRecord(TitleMenuRecordView *record, int mode);
};
extern ReplayRecordRegistryView g_ReplayRecordRegistry;

TitleMenuRecordView *__stdcall CreateReplayRecording(int mode);
TitleMenuRecordView *__stdcall CreateReplayPlayback(const char *filename);
