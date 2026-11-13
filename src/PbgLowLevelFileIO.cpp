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

// Native complete 0x441EF0..0x441F08, vtable slot +0x10.
// Invalid files report offset zero; otherwise SetFilePointer seeks relative
// to the current position without changing it.
unsigned int PbgLowLevelFileView::Tell()
{
    if (handle == INVALID_HANDLE_VALUE) return 0;
    return SetFilePointer(handle, 0, 0, FILE_CURRENT);
}

// 0x441F10..0x441F24: invalid handles have size zero, not INVALID_FILE_SIZE.
unsigned int PbgLowLevelFileView::Size()
{
    if (handle == INVALID_HANDLE_VALUE)
        return 0;
    return GetFileSize(handle, 0);
}

// 0x441F30..0x441F54: only AL reports success. The native leaves the
// remaining EAX bits unspecified; claiming an unsigned int return was what
// introduced nine extra bytes and also made ReadWhole test EAX instead of AL.
unsigned char PbgLowLevelFileView::Seek(int offset, unsigned int origin)
{
    if (handle == INVALID_HANDLE_VALUE) return 0;
    SetFilePointer(handle, offset, 0, origin);
    return 1;
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
    typedef unsigned char (__thiscall *SeekTo)(PbgLowLevelFileView *, int, unsigned int);
    typedef unsigned int (__thiscall *ReadBytes)(PbgLowLevelFileView *, void *, unsigned int);
    unsigned int length = reinterpret_cast<GetSize>(vtable[5])(this);
    if (length > maximumSize)
        return 0;
    void *buffer = malloc(length);
    if (!buffer)
        return 0;
    unsigned int pos = reinterpret_cast<Tell>(vtable[4])(this);
    if (!reinterpret_cast<SeekTo>(vtable[6])(this, pos, 0))
        return 0;
    if (!reinterpret_cast<ReadBytes>(vtable[2])(this, buffer, length))
    {
        free(buffer);
        return 0;
    }
    reinterpret_cast<SeekTo>(vtable[6])(this, pos, 0);
    return buffer;
}
