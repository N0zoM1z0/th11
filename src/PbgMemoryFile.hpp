#pragma once
#include <stddef.h>

// Physical Pbg::MemFile (0x10 bytes). Original vtable at 0x4965CC, base
// IFile vtable 0x496514; neither owner translation unit is yet identified.
struct PbgMemoryFileView
{
    const unsigned int *vtable;    // +0
    unsigned int length;          // +4
    unsigned char *cursor;        // +8
    unsigned char *buffer;        // +0xC

    void Reset();
    PbgMemoryFileView *DestroyAndMaybeDelete(unsigned int flags);
    ~PbgMemoryFileView();
    unsigned char Open(const char *filename, const char *mode);
    unsigned int Read(void *destination, unsigned int length);
    unsigned int Tell() const;
    unsigned int Size() const;
    unsigned char Seek(int offset, unsigned int origin);
    unsigned char Write(const void *data, unsigned int length);
};
typedef char PbgMemoryFileSize[(sizeof(PbgMemoryFileView) == 0x10) ? 1 : -1];
typedef char PbgMemoryFileCursorOffset[(offsetof(PbgMemoryFileView,cursor)==8) ? 1 : -1];
