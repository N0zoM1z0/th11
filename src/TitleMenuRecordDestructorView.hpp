#pragma once
#include "Chain.hpp"
#include "ReplayRecordStageLink.hpp"
#include <stddef.h>

// POD-only physical view used to spell the native naked destructor body.
// The real game-facing TitleMenuRecordView retains ReplayRecordStageLink[8];
// this view is layout-identical and the owner explicitly invokes the native
// vector destructor callback once its SEH-protected cleanup is complete.
struct TitleRecordDestructorSegmentStream
{
    void *first, *middle, *last;
};
struct TitleRecordDestructorStageStorage
{
    unsigned char *eventsBegin;
    unsigned char *eventsBeginCopy;
    unsigned char *eventsEnd;
    unsigned char *eventsEndCopy;
    void *stageHeader;
    unsigned int unknown014;
    ReplayRecordStageNode link;
};
typedef char TitleRecordDestructorStageSize[(sizeof(TitleRecordDestructorStageStorage) == 0x24) ? 1 : -1];

struct TitleMenuRecordDestructorView
{
    unsigned char unknown000[8];
    ChainElement *updateChain, *drawChain;
    unsigned int unknown010;
    void *entryData;
    void *buffer018;
    void *buffers[8];
    unsigned int unknown03c;
    TitleRecordDestructorSegmentStream streams[8];
    unsigned int unknown0a0;
    TitleRecordDestructorStageStorage stages[8];
    unsigned char unknown1c4[0x1d0 - 0x1c4];
    ChainElement *secondaryChain;
    unsigned char unknown1d4[0x2dc - 0x1d4];
};
typedef char TitleRecordDestructorSize[(sizeof(TitleMenuRecordDestructorView) == 0x2dc) ? 1 : -1];
typedef char TitleRecordDestructorStageOffset[(offsetof(TitleMenuRecordDestructorView, stages) == 0xa4) ? 1 : -1];
