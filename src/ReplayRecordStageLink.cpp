#include "ReplayRecordStageLink.hpp"

// 0x4374D0..0x4374F4: independently bounded release of the embedded intrusive
// stage node. No deletion or allocation occurs in this 37-byte owner.
ReplayRecordStageLink::~ReplayRecordStageLink()
{
    if (link.previous) link.previous->next = link.next;
    if (link.next) link.next->previous = link.previous;
    link.previous = 0;
    link.next = 0;
}
