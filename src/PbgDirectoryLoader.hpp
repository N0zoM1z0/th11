#pragma once
#include "PbgResourceSession.hpp"
#include <stddef.h>

// The Pbg THA1 fixed header is read as four DWORDs through a 16-byte stream
// Read call. Its original encrypted fields are decoded in place.
struct PbgDirectoryHeaderView
{
    unsigned int signature;
    unsigned int directoryOffset;
    unsigned int compressedLength;
    unsigned int entryCount;
};
typedef char PbgDirectoryHeaderSize[(sizeof(PbgDirectoryHeaderView)==16)?1:-1];

unsigned char __fastcall LoadPbgSessionDirectory(PbgResourceSessionView *session,
    const char *filename);
