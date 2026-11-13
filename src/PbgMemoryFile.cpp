#include "PbgMemoryFile.hpp"
#include "PbgResFileRuntime.hpp"
#include <new>
#include <stdlib.h>
#include <string.h>

extern const unsigned int g_PbgMemFileVtable[];
extern const unsigned int g_PbgIFileVtable[];

// Original 0x442C20: write is unsupported; two stack arguments are popped.
unsigned char PbgMemoryFileView::Write(const void *, unsigned int)
{
    return 0;
}

// Original 0x442CA0: the independent non-deleting owner frees the buffer,
// clears buffer/size/cursor and reinstalls the base IFile virtual table.
PbgMemoryFileView::~PbgMemoryFileView()
{
    vtable = g_PbgMemFileVtable;
    if (buffer)
    {
        free(buffer);
        buffer = 0;
    }
    buffer = 0;
    length = 0;
    cursor = 0;
    vtable = g_PbgIFileVtable;
}

// Original 0x442C50: independent scalar-deleting owner with the same real
// payload lifetime followed by conditional storage deletion (flags & 1).
PbgMemoryFileView *PbgMemoryFileView::DestroyAndMaybeDelete(unsigned int flags)
{
    vtable = g_PbgMemFileVtable;
    if (buffer)
    {
        free(buffer);
        buffer = 0;
    }
    buffer = 0;
    length = 0;
    cursor = 0;
    vtable = g_PbgIFileVtable;
    if (flags & 1u) ::operator delete(this);
    return this;
}

// Native 0x442D10: clear memory storage without modifying the vtable.
void PbgMemoryFileView::Reset()
{
    if (buffer)
    {
        free(buffer);
        buffer = 0;
    }
    buffer = 0;
    length = 0;
    cursor = 0;
}

// Native 0x442CE0: load requested named resource and its uncompressed size.
unsigned char PbgMemoryFileView::Open(const char *filename)
{
    buffer = static_cast<unsigned char *>(ReadPbgResourceByName(filename));
    length = GetPbgResourceSizeByName(filename);
    cursor = buffer;
    return buffer != 0;
}

// Native 0x442D40: copy as much as exists and advance the read cursor.
unsigned int PbgMemoryFileView::Read(void *destination, unsigned int requested)
{
    unsigned int available = static_cast<unsigned int>(buffer + length - cursor);
    if (requested <= available)
    {
        memcpy(destination, cursor, requested);
        cursor += requested;
        return requested;
    }
    if (available != 0)
    {
        memcpy(destination, cursor, available);
        cursor += available;
        return available;
    }
    return 0;
}

// Native 0x442DA0 / 0x442DB0, physical read cursor and total length.
unsigned int PbgMemoryFileView::Tell() const
{
    return static_cast<unsigned int>(cursor - buffer);
}
unsigned int PbgMemoryFileView::Size() const
{
    return length;
}

// Native 0x442DC0: absolute/relative/end seeks with strict bounds. The
// original's result is AL-only (other EAX bits have no semantic meaning).
unsigned char PbgMemoryFileView::Seek(int offset, unsigned int origin)
{
    if (origin == 0)
    {
        if (offset >= 0 && static_cast<unsigned int>(offset) < length)
        {
            cursor = buffer + offset;
            return 1;
        }
    }
    else if (origin == 1)
    {
        int remaining = static_cast<int>(buffer + length - cursor);
        if (offset < remaining)
        {
            cursor += offset;
            return 1;
        }
    }
    else if (origin == 2 && offset <= 0)
    {
        unsigned int distance = static_cast<unsigned int>(offset < 0 ? -offset : offset);
        if (distance < length)
        {
            cursor = buffer + length + offset;
            return 1;
        }
    }
    return 0;
}
