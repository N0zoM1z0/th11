#include "PbgLowLevelFile.hpp"
#include <new>

extern const unsigned int g_PbgFileVtable[];
extern const unsigned int g_PbgIFileVtable[];

// Complete native 0x441D20..0x441D4D: transition through derived and base
// vtables, close the real Win32 handle only when it is not INVALID_HANDLE_VALUE.
PbgLowLevelFileView::~PbgLowLevelFileView()
{
    vtable = g_PbgFileVtable;
    if (handle != INVALID_HANDLE_VALUE)
    {
        CloseHandle(handle);
        handle = INVALID_HANDLE_VALUE;
        access = 0;
    }
    vtable = g_PbgIFileVtable;
}

// Original 0x441CD0..0x441D11: the scalar deleting destructor performs the
// same close and vtable lifetime and conditionally frees storage on bit 0.
PbgLowLevelFileView *PbgLowLevelFileView::DestroyAndMaybeDelete(unsigned int flags)
{
    vtable = g_PbgFileVtable;
    if (handle != INVALID_HANDLE_VALUE)
    {
        CloseHandle(handle);
        handle = INVALID_HANDLE_VALUE;
        access = 0;
    }
    vtable = g_PbgIFileVtable;
    if (flags & 1u)
        ::operator delete(this);
    return this;
}

// Original 0x441D50..0x441E3D: native mode selection and Win32 file creation
// with a 260-character resolved pathname and compiler cookie frame.
unsigned char PbgLowLevelFileView::Open(const char *filename, const char *mode)
{
    unsigned int creation = 0;
    int append = 0;
    char resolvedPath[MAX_PATH];
    typedef void (__thiscall *Close)(PbgLowLevelFileView *);
    reinterpret_cast<Close>(vtable[1])(this);
    const char *cursor = mode;
    for (;;)
    {
        char ch = *cursor;
        if (!ch) return 0;
        if (ch == 'r')
        {
            access = GENERIC_READ;
            creation = OPEN_EXISTING;
            break;
        }
        if (ch == 'w')
        {
            DeleteFileA(filename);
            access = GENERIC_WRITE;
            creation = CREATE_ALWAYS;
            break;
        }
        if (ch == 'a')
        {
            append = 1;
            access = GENERIC_WRITE;
            creation = OPEN_ALWAYS;
            break;
        }
        ++cursor;
    }
    ResolvePbgDiskPath(resolvedPath, filename);
    handle = CreateFileA(resolvedPath, access, FILE_SHARE_READ, 0,
        creation, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, 0);
    if (handle == INVALID_HANDLE_VALUE) return 0;
    if (append) SetFilePointer(handle, 0, 0, FILE_END);
    return 1;
}
