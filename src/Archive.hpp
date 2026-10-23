#pragma once

// Partial virtual stream interface; only the observed Read/Seek slots are named.
class ArchiveStreamView
{
public:
    virtual void Unknown00() = 0;
    virtual void Unknown04() = 0;
    virtual int Read(void *buffer, int size) = 0;
    virtual void Unknown0c() = 0;
    virtual void Unknown10() = 0;
    virtual void Unknown14() = 0;
    virtual bool Seek(unsigned int offset, int origin) = 0;
};

struct ArchiveEntryView
{
    const char *name;
    unsigned int offset;
    unsigned int size;
    unsigned int unknown0c;
};

struct ArchiveStateView
{
    ArchiveEntryView *entries;
    int entryCount;
    const char *filename;
    ArchiveStreamView *stream;
};

struct ArchiveCipherRule
{
    unsigned char key;
    unsigned char step;
    unsigned short unknown02;
    int blockSize;
    int limit;
};

typedef char ArchiveEntryViewSize[(sizeof(ArchiveEntryView) == 16) ? 1 : -1];
typedef char ArchiveStateViewSize[(sizeof(ArchiveStateView) == 16) ? 1 : -1];
typedef char ArchiveCipherRuleSize[(sizeof(ArchiveCipherRule) == 12) ? 1 : -1];

extern ArchiveStateView g_GameArchive;
extern ArchiveCipherRule g_ArchiveCipherRules[8];

ArchiveEntryView *__stdcall FindArchiveEntry(ArchiveStateView *self, const char *name);
unsigned char *__stdcall ReadArchivedFile(ArchiveStateView *self, const char *name,
                                          unsigned char *destination);
// The observed private convention puts outputSize in EAX and leaves the
// other three arguments on the stack. The public source uses ordinary stdcall.
unsigned char *__stdcall DecompressArchiveData(unsigned int outputSize,
                                               unsigned char *data, int size,
                                               unsigned char *destination);
void __cdecl ArchiveTrace(const char *format, ...);
