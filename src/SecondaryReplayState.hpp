#pragma once
#include "Chain.hpp"
#include "ReplayRecordStageLink.hpp"
#include <stddef.h>

// Replay stage headers are the 0x90-byte records embedded in the unpacked
// replay payload. The same layout is allocated separately while recording.
struct ReplayStageHeaderView
{
    unsigned char bytes[0x90];
};
struct ReplayStageStreamView
{
    void *first;
    void *middle;
    void *last;
};

// Shared physical state used by the replay initializer, frame recorder, file
// loader, stage-transition owner, and writer. The three pointer arrays have
// distinct roles: recording-stage allocations at +0x1C, intrusive segment
// list heads at +0x40, and playback-stage links at +0xA4.
struct ReplayStateView
{
    unsigned char unknown000[8];
    ChainElement *update;
    ChainElement *draw;
    int mode;
    void *fileHeader;
    void *header;
    void *stages[8];
    unsigned char unknown03c[4];
    ReplayStageStreamView streams[8];
    int segmentCount;
    ReplayRecordStageLink stageLinks[8];
    unsigned char *unpackedData;
    unsigned int unknown1c8;
    int frame;
    ChainElement *secondaryUpdate;
    int recordingStage;
    unsigned int flags;
};

typedef ReplayStateView ReplayWriteState;
typedef ReplayStateView ReplayInitStateView;
extern ReplayStateView *g_SecondaryReplayCurrent;

typedef char ReplayStateHeaderOffset[(offsetof(ReplayStateView, header) == 0x18) ? 1 : -1];
typedef char ReplayStateStagesOffset[(offsetof(ReplayStateView, stages) == 0x1c) ? 1 : -1];
typedef char ReplayStateStreamsOffset[(offsetof(ReplayStateView, streams) == 0x40) ? 1 : -1];
typedef char ReplayStateCurrentSegmentOffset[(offsetof(ReplayStateView, streams) + 7 * sizeof(ReplayStageStreamView) + 8) == 0x9c ? 1 : -1];
typedef char ReplayStateSegmentCountOffset[(offsetof(ReplayStateView, segmentCount) == 0xa0) ? 1 : -1];
typedef char ReplayStateStageLinksOffset[(offsetof(ReplayStateView, stageLinks) == 0xa4) ? 1 : -1];
typedef char ReplayStateUnpackedOffset[(offsetof(ReplayStateView, unpackedData) == 0x1c4) ? 1 : -1];
typedef char ReplayStateFrameOffset[(offsetof(ReplayStateView, frame) == 0x1cc) ? 1 : -1];
typedef char ReplayStateSecondaryUpdateOffset[(offsetof(ReplayStateView, secondaryUpdate) == 0x1d0) ? 1 : -1];
typedef char ReplayStateRecordingStageOffset[(offsetof(ReplayStateView, recordingStage) == 0x1d4) ? 1 : -1];
typedef char ReplayStateFlagsOffset[(offsetof(ReplayStateView, flags) == 0x1d8) ? 1 : -1];
