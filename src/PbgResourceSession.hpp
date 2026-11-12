#pragma once
#include <stddef.h>

// Four physical DWORD fields are observed in the original Pbg file/resource
// owner at 0x4415A0. The original nominal C++ class name is not established.
struct PbgResourceSessionView
{
    unsigned int entries;
    unsigned int count;
    unsigned int temporary;
    unsigned int stream;

    PbgResourceSessionView();
    ~PbgResourceSessionView();
};
typedef char PbgResourceSessionSize[(sizeof(PbgResourceSessionView) == 0x10) ? 1 : -1];
typedef char PbgSessionTemporaryOffset[(offsetof(PbgResourceSessionView, temporary) == 8) ? 1 : -1];
typedef char PbgSessionStreamOffset[(offsetof(PbgResourceSessionView, stream) == 0xc) ? 1 : -1];

void __fastcall ReleasePbgResourceSession(PbgResourceSessionView *self);
unsigned char __fastcall OpenPbgFileSession(PbgResourceSessionView *self, const char *path);
unsigned char __fastcall OpenPbgResFileSession(PbgResourceSessionView *self, const char *path);
