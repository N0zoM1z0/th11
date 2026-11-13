#pragma once
#include <stddef.h>

// The indexed replay stage has a 12-byte intrusive node after 24 bytes of
// stage payload. Its +4/+8 predecessor/successor fields are physical facts.
struct ReplayRecordStageNode
{
    void *owner;
    ReplayRecordStageNode *previous, *next;
};
struct ReplayRecordStageLink
{
    unsigned char *eventsBegin;
    unsigned char *eventsBeginCopy;
    unsigned char *eventsEnd;
    unsigned char *eventsEndCopy;
    void *stageHeader;
    unsigned int unknown014;
    ReplayRecordStageNode link;
    ReplayRecordStageLink();
    ~ReplayRecordStageLink();
};
typedef char ReplayStageLinkSize[(sizeof(ReplayRecordStageLink) == 0x24) ? 1 : -1];
typedef char ReplayStageLinkPrevOffset[(offsetof(ReplayRecordStageLink, link) == 0x18) ? 1 : -1];
typedef char ReplayStageLinkHeaderOffset[(offsetof(ReplayRecordStageLink, stageHeader) == 0x10) ? 1 : -1];
