#pragma once
#include "ReplayInputRepeat.hpp"
#include <stddef.h>

// Physical owner of the replay event stream written by native 0x435FE0.
struct ReplayFrameSegment
{
    unsigned char eventPayload[0x1518];
    unsigned char *eventEnd;
    unsigned char unknown151c[0x18a0 - 0x151c];
    unsigned char *sampleEnd;
};
struct ReplayFrameSegmentNode
{
    ReplayFrameSegment *segment;
    ReplayFrameSegmentNode *next, *previous;
};
struct ReplayFrameRecordingView
{
    unsigned char unknown000[0x9c];
    ReplayFrameSegmentNode *last;
    unsigned char unknown0a0[0x1cc - 0xa0];
    int frame;
    unsigned int unknown1d0;
    int stage;
};
struct ReplayFramePlayerSampleView
{
    unsigned char unknown000[0x34];
    float sample;
};

typedef char ReplayRecordSegmentEventOffset[(offsetof(ReplayFrameSegment, eventEnd) == 0x1518) ? 1 : -1];
typedef char ReplayRecordSegmentSampleOffset[(offsetof(ReplayFrameSegment, sampleEnd) == 0x18a0) ? 1 : -1];
typedef char ReplayRecordLastOffset[(offsetof(ReplayFrameRecordingView, last) == 0x9c) ? 1 : -1];
typedef char ReplayRecordFrameOffset[(offsetof(ReplayFrameRecordingView, frame) == 0x1cc) ? 1 : -1];
typedef char ReplayRecordStageOffset[(offsetof(ReplayFrameRecordingView, stage) == 0x1d4) ? 1 : -1];
int __fastcall RecordReplayInputFrame(ReplayFrameRecordingView *self);
