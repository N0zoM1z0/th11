#include "PbgResourceSession.hpp"
#include "PbgResourceEntry.hpp"
#include <new>
#include <stdlib.h>

// The observed Pbg::File and Pbg::ResFile C++ vtable addresses are physical
// roots, not proof of a matching original source declaration for our session.
extern const unsigned int g_PbgFileVtable[];
extern const unsigned int g_PbgResFileVtable[];
extern const char g_PbgFileReadMode[];
extern const char g_PbgFileOpenLog[], g_PbgFileErrorLog[];
extern const char g_PbgSessionCleanupLog[];

struct PbgNativeFileView
{
    const unsigned int *vtable;
    int stream;
    unsigned int flags;
    PbgNativeFileView() : vtable(g_PbgFileVtable), stream(-1), flags(0) {}
};
struct PbgNativeResFileView
{
    const unsigned int *vtable;
    unsigned int field004, field008, field00c;
    PbgNativeResFileView() : vtable(g_PbgResFileVtable), field004(0), field008(0), field00c(0) {}
};

typedef char PbgNativeFileSize[(sizeof(PbgNativeFileView) == 0xc) ? 1 : -1];
typedef char PbgNativeResFileSize[(sizeof(PbgNativeResFileView) == 0x10) ? 1 : -1];

// Reviewed calls are genuine separate native helper owners; their private
// argument registers and original TU partition remain open.
void __cdecl TracePbgResourceEvent(const char *format, ...);
unsigned char __fastcall LoadPbgSessionDirectory(PbgResourceSessionView *self, const char *filename);
char *__stdcall DuplicatePbgResourcePath(const char *filename);

namespace
{
__forceinline PbgNativeFileView *FileAt(unsigned int value)
{
    return reinterpret_cast<PbgNativeFileView *>(value);
}
__forceinline void DeletePolymorphicPbgStream(unsigned int handle)
{
    if (!handle) return;
    PbgNativeFileView *stream = FileAt(handle);
    typedef void (__thiscall *DeletingDtor)(PbgNativeFileView *, unsigned int);
    reinterpret_cast<DeletingDtor>(stream->vtable[7])(stream, 1);
}
__forceinline void OpenFileStream(PbgNativeFileView *stream,
    const char *filename, const char *mode)
{
    typedef int (__thiscall *Open)(PbgNativeFileView *, const char *, const char *);
    reinterpret_cast<Open>(stream->vtable[0])(stream, filename, mode);
}
}

// Native 0x4416C0..0x44172C, a substantive cleanup of stream, temporary
// buffers and the 16-byte Pbg directory-element array. The native ESI
// receiver is private linked codegen, not an artificial source convention.
void __fastcall ReleasePbgResourceSession(PbgResourceSessionView *self)
{
    if (self->temporary)
    {
        TracePbgResourceEvent(g_PbgSessionCleanupLog,
            reinterpret_cast<void *>(self->temporary));
        if (self->temporary)
        {
            free(reinterpret_cast<void *>(self->temporary));
            self->temporary = 0;
        }
    }
    // The native writes this again after the optional free.
    self->temporary = 0;
    if (self->entries)
    {
        delete [] reinterpret_cast<PbgResourceEntryView *>(self->entries);
    }
    self->entries = 0;
    DeletePolymorphicPbgStream(self->stream);
    // VC9 emits the original count-then-stream stores from these independent
    // ordinary assignments; both fields are zero at function exit.
    self->stream = 0;
    self->count = 0;
}

// Native 0x4415B0..0x4415B9: ESI-preserving ten-byte forwarding adapter.
PbgResourceSessionView::~PbgResourceSessionView()
{
    ReleasePbgResourceSession(this);
}

// Native 0x4415C0..0x441645 (134 bytes). File stream at +C is 12 bytes,
// opened with the named mode after the shared session loader and string copy.
unsigned char __fastcall OpenPbgFileSession(PbgResourceSessionView *self,
    const char *filename)
{
    ReleasePbgResourceSession(self);
    TracePbgResourceEvent(g_PbgFileOpenLog, filename);
    PbgNativeFileView *stream = new PbgNativeFileView;
    self->stream = reinterpret_cast<unsigned int>(stream);
    if (stream)
    {
        if (LoadPbgSessionDirectory(self, filename))
        {
            char *copied = DuplicatePbgResourcePath(filename);
            self->temporary = reinterpret_cast<unsigned int>(copied);
            if (copied)
            {
                OpenFileStream(stream, copied, g_PbgFileReadMode);
                return 1;
            }
        }
        TracePbgResourceEvent(g_PbgFileErrorLog, filename);
        ReleasePbgResourceSession(self);
    }
    return 0;
}

// Native 0x441650..0x4416B4 (101 bytes). ResFile reserves four DWORDs and
// accepts a non-null copied path without invoking the virtual open method.
unsigned char __fastcall OpenPbgResFileSession(PbgResourceSessionView *self,
    const char *filename)
{
    ReleasePbgResourceSession(self);
    PbgNativeResFileView *stream = new PbgNativeResFileView;
    self->stream = reinterpret_cast<unsigned int>(stream);
    if (stream)
    {
        if (LoadPbgSessionDirectory(self, filename))
        {
            char *copied = DuplicatePbgResourcePath(filename);
            self->temporary = reinterpret_cast<unsigned int>(copied);
            if (copied) return 1;
        }
        ReleasePbgResourceSession(self);
    }
    return 0;
}
