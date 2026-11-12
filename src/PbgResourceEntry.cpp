#include "PbgResourceEntry.hpp"
#include <stdlib.h>

// Native full 0x441C00..0x441C19: the zeroing belongs to the non-null arm,
// even when the caller is an EH vector iterator over many entries.
PbgResourceEntryView::~PbgResourceEntryView()
{
    if (name)
    {
        free(name);
        name = 0;
    }
}
