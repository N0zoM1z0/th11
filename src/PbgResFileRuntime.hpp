#pragma once
#include <stddef.h>

// The 16-byte Pbg::ResFile native vtable-backed object is separate from the
// four-DWORD resource session that owns it. Original nominal class/TU unknown.
struct PbgResFileRuntimeView
{
    const unsigned int *vtable;
    unsigned int unknown004, unknown008, unknown00c;
    PbgResFileRuntimeView *DestroyAndMaybeDelete(unsigned int flags);
};
typedef char PbgResFileRuntimeSize[(sizeof(PbgResFileRuntimeView)==0x10)?1:-1];

// The original resource index is 16-byte entries starting at physical
// 0x4C3C48; the count lives at 0x4A8ED0.
struct PbgResResourceRecordView
{
    unsigned int unknown00, unknown04;
    const char *filename;
    unsigned int unknown0c;
};
typedef char PbgResResourceRecordSize[(sizeof(PbgResResourceRecordView)==0x10)?1:-1];

extern int g_PbgResFileCount;
extern PbgResResourceRecordView g_PbgResFileEntries[];

PbgResResourceRecordView *__fastcall FindPbgResourceRecord(const char *filename);
void *__fastcall ReadPbgResourceByName(const char *filename);
unsigned int __fastcall GetPbgResourceSizeByName(const char *filename);
