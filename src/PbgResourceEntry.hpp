#pragma once
#include <stddef.h>

// Physical 16-byte Pbg directory entry. In the original each allocated entry
// participates in the EH vector destructor called by the session cleanup.
struct PbgResourceEntryView
{
    char *name;
    unsigned int unknown004, unknown008, unknown00c;
    ~PbgResourceEntryView();
};
typedef char PbgResourceEntrySize[(sizeof(PbgResourceEntryView) == 0x10) ? 1 : -1];
