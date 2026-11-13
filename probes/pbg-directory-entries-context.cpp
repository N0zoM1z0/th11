#include "PbgDirectoryEntries.hpp"
#include "PbgDirectoryLoader.hpp"

// Bounded genuine caller: the original 336-byte loader supplies the decoded
// directory bytes, number of entries and the post-directory sentinel offset.
extern "C" PbgResourceEntryView *__stdcall PbgDirectoryEntriesProbe(
    const unsigned char *data, unsigned int count, unsigned int endOffset)
{
    return ParsePbgDirectoryEntries(data, count, endOffset);
}
