#pragma once

#include "Chain.hpp"
#include "ReplayRecordStageLink.hpp"
#include <stddef.h>
#include <string.h>

// Physical view of the 429-byte replay record owner at 0x435C50.
// The 8x12-byte segment streams at +0x40 are also consumed by the
// independently exact ClearReplayStageSegments helper.
struct TitleReplaySegmentStream
{
    void *first, *middle, *last;
};
struct TitleMenuRecordView
{
    unsigned char unknown000[8];
    ChainElement *updateChain, *drawChain;
    unsigned int unknown010;
    void *entryData;
    void *buffer018;
    void *buffers[8];
    unsigned int unknown03c;
    TitleReplaySegmentStream streams[8];
    unsigned int unknown0a0;
    ReplayRecordStageLink stages[8];
    unsigned char unknown1c4[0x1d0 - 0x1c4];
    ChainElement *secondaryChain;
    unsigned char unknown1d4[0x2dc - 0x1d4];
    ~TitleMenuRecordView();
};

typedef char TitleReplayRecordStreamOffset[(offsetof(TitleMenuRecordView, streams) == 0x40) ? 1 : -1];
typedef char TitleReplayRecordStagesOffset[(offsetof(TitleMenuRecordView, stages) == 0xa4) ? 1 : -1];
typedef char TitleReplayRecordSecondaryOffset[(offsetof(TitleMenuRecordView, secondaryChain) == 0x1d0) ? 1 : -1];

typedef char ReplayRecordStorageSize[(sizeof(TitleMenuRecordView) == 0x2dc) ? 1 : -1];
