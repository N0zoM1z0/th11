#pragma once
#include "PbgResourceEntry.hpp"
#include <stddef.h>

// One directory entry is 16 bytes; a (count+1)th sentinel records the packed
// archive directory's physical end offset in its metadata at +4.
PbgResourceEntryView *__stdcall ParsePbgDirectoryEntries(
    const unsigned char *data, unsigned int count, unsigned int tailOffset);
