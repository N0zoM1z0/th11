#include "PbgWinResourceMemoryFile.hpp"
#include <new>
#include <stdlib.h>
#include <string.h>

extern const unsigned int g_PbgMemFileVtable[];
extern const unsigned int g_PbgIFileVtable[];

// Original 0x442F90..0x442FC0: the Win32 resource stream has its *own*
// non-deleting destructor despite equivalent physical MemFile cleanup.
PbgWinResourceMemoryFileView::~PbgWinResourceMemoryFileView()
{
    vtable = g_PbgMemFileVtable;
    if (buffer)
    {
        free(buffer);
        buffer = 0;
    }
    buffer = 0;
    size = 0;
    cursor = 0;
    vtable = g_PbgIFileVtable;
}

// Original 0x442F40..0x442F84: same buffer lifetime, then flag-gated
// scalar delete. This is a separate unique 69-byte vtable owner.
PbgWinResourceMemoryFileView *
PbgWinResourceMemoryFileView::DestroyAndMaybeDelete(unsigned int flags)
{
    vtable = g_PbgMemFileVtable;
    if (buffer)
    {
        free(buffer);
        buffer = 0;
    }
    buffer = 0;
    size = 0;
    cursor = 0;
    vtable = g_PbgIFileVtable;
    if (flags & 1u) ::operator delete(this);
    return this;
}

namespace
{
__forceinline void CloseNativePbgResourceStream(PbgWinResourceMemoryFileView *self)
{
    typedef void (__thiscall *Close)(PbgWinResourceMemoryFileView *);
    reinterpret_cast<Close>(self->vtable[1])(self);
}
}

// Original complete 169-byte 0x442FD0..0x443078. The native discards the
// second stack argument and ends with RET8. All native outcome branches
// return AL=1, including resource lookup/alloc errors; the class owns the
// lifetime cleanup through vtable slot1 on failure.
unsigned char PbgWinResourceMemoryFileView::OpenResource(
    const char *name, const char *unusedMode)
{
    (void)unusedMode;
    CloseNativePbgResourceStream(this);
    HRSRC resource = FindResourceA(0, name, MAKEINTRESOURCEA(10));
    if (resource)
    {
        HGLOBAL handle = LoadResource(0, resource);
        if (handle)
        {
            const void *readOnly = LockResource(handle);
            if (!readOnly)
            {
                FreeResource(0);
                CloseNativePbgResourceStream(this);
                return 1;
            }
            size = SizeofResource(0, resource);
            buffer = static_cast<unsigned char *>(malloc(size));
            if (buffer)
            {
                memcpy(buffer, readOnly, size);
                cursor = buffer;
                FreeResource(handle);
                return 1;
            }
        }
    }
    CloseNativePbgResourceStream(this);
    return 1;
}
