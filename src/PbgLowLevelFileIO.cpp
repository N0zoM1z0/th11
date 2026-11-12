#include "PbgLowLevelFile.hpp"
#include <stdlib.h>

// 0x441E40..0x441E61: caller-visible close, with no vtable state transition.
void PbgLowLevelFileView::Close()
{
    if (handle != INVALID_HANDLE_VALUE)
    {
        CloseHandle(handle);
        handle = INVALID_HANDLE_VALUE;
        access = 0;
    }
}

// 0x441E70..0x441EA8: return ReadFile's byte count rather than its BOOL.
unsigned int PbgLowLevelFileView::Read(void *destination, unsigned int length)
{
    DWORD count = 0;
    if (access != GENERIC_READ)
        return 0;
    ReadFile(handle, destination, length, &count, 0);
    return count;
}

// 0x441EB0..0x441EEE: true only if the full requested length was written.
unsigned char PbgLowLevelFileView::Write(const void *source, unsigned int length)
{
    DWORD written = 0;
    if (access != GENERIC_WRITE)
        return false;
    WriteFile(handle, source, length, &written, 0);
    return length == written;
}

// 0x441F10..0x441F24: invalid handles have size zero, not INVALID_FILE_SIZE.
unsigned int PbgLowLevelFileView::Size()
{
    if (handle == INVALID_HANDLE_VALUE)
        return 0;
    return GetFileSize(handle, 0);
}

// 0x441F30..0x441F54: the native return preserves the SetFilePointer upper
// 24 bits, overwrites only the lowest byte with 1 (or returns 0xFFFFFF00).
unsigned int PbgLowLevelFileView::Seek(int offset, unsigned int origin)
{
    if (handle == INVALID_HANDLE_VALUE)
        return 0xffffff00u;
    unsigned int result = SetFilePointer(handle, offset, 0, origin);
    return (result & 0xffffff00u) | 1u;
}

// 0x441F60..0x441FF0: the Pbg stream owns a virtual Size/Tell/Seek/Read
// protocol. Retain the native failure paths: a failed virtual seek before
// reading returns NULL without an invented compensating free.
void *PbgLowLevelFileView::ReadWhole(unsigned int maximumSize)
{
    if (access != GENERIC_READ)
        return 0;

    typedef unsigned int (__thiscall *GetSize)(PbgLowLevelFileView *);
    typedef unsigned int (__thiscall *Tell)(PbgLowLevelFileView *);
    typedef unsigned int (__thiscall *SeekTo)(PbgLowLevelFileView *, int, unsigned int);
    typedef unsigned int (__thiscall *ReadBytes)(PbgLowLevelFileView *, void *, unsigned int);
    unsigned int length = reinterpret_cast<GetSize>(vtable[5])(this);
    if (maximumSize < length)
        return 0;
    void *buffer = malloc(length);
    if (!buffer)
        return 0;
    unsigned int pos = reinterpret_cast<Tell>(vtable[4])(this);
    if (!reinterpret_cast<SeekTo>(vtable[6])(this, pos, 0))
        return 0;
    if (reinterpret_cast<ReadBytes>(vtable[2])(this, buffer, length))
    {
        reinterpret_cast<SeekTo>(vtable[6])(this, pos, 0);
        return buffer;
    }
    free(buffer);
    return 0;
}
