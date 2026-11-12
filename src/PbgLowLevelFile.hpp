#pragma once
#include <windows.h>
#include <stddef.h>

// Physical Pbg::File stream layout from native vtable and CloseHandle paths.
// Historic class/TU names and compiler-generated scalar delete linkage are
// indeterminate even though both physical vtable addresses are reviewed.
struct PbgLowLevelFileView
{
    const unsigned int *vtable;
    HANDLE handle;
    unsigned int access;

    ~PbgLowLevelFileView();
    PbgLowLevelFileView *DestroyAndMaybeDelete(unsigned int flags);
    void Open(const char *filename, const char *mode);
};
typedef char PbgLowLevelFileSize[(sizeof(PbgLowLevelFileView) == 0xc) ? 1 : -1];
typedef char PbgLowLevelFileHandleOffset[(offsetof(PbgLowLevelFileView, handle) == 4) ? 1 : -1];

void __fastcall ResolvePbgDiskPath(char *destination, const char *filename);
