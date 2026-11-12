#include "ReplayRecordStageLink.hpp"

// Native complete 0x437490..0x4374C2: nine separate, observable dword resets
// precede the cursor copies and intrusive-node ownership publication. An
// ordinary array loop folds two observed memory operations; this typed
// volatile view preserves the original field-write/read sequence, not padding.
ReplayRecordStageLink::ReplayRecordStageLink()
{
    volatile unsigned int *fields =
        reinterpret_cast<volatile unsigned int *>(this);
    fields[0] = 0;
    fields[1] = 0;
    fields[2] = 0;
    fields[3] = 0;
    fields[4] = 0;
    fields[5] = 0;
    fields[6] = 0;
    fields[7] = 0;
    fields[8] = 0;
    fields[1] = fields[0];
    fields[3] = fields[2];
    link.owner = this;
    link.previous = 0;
    link.next = 0;
}
