#include "ReplayRecordStageLink.hpp"
#include <string.h>

// 00437490. Zero the whole link, copy the two cursors, then publish the node.
ReplayRecordStageLink::ReplayRecordStageLink()
{
    memset(this, 0, sizeof(*this));
    eventsBeginCopy = eventsBegin;
    eventsEndCopy = eventsEnd;
    link.owner = this;
    link.previous = 0;
    link.next = 0;
}
