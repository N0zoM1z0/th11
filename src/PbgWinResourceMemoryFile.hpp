#pragma once
#include <windows.h>
#include <stddef.h>

// Separate Win32 RCDATA-backed Pbg stream vtable root at 0x496614. Its
// 0x10-byte physical layout intentionally agrees with Pbg::MemFile, but it
// has distinct *owned* destructor and resource-loading functions.
struct PbgWinResourceMemoryFileView
{
    const unsigned int *vtable;
    unsigned int size;
    unsigned char *cursor;
    unsigned char *buffer;

    ~PbgWinResourceMemoryFileView();
    PbgWinResourceMemoryFileView *DestroyAndMaybeDelete(unsigned int flags);
    unsigned char OpenResource(const char *name, const char *unusedMode);
};
typedef char PbgWinResourceMemoryFileSize[(sizeof(PbgWinResourceMemoryFileView)==0x10)?1:-1];
