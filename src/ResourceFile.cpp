#include "ResourceFile.hpp"
#include "Archive.hpp"
#include "SharedFile.hpp"
#include "RuntimeCritical.hpp"

#include <stdlib.h>
#include <string.h>

void __cdecl InputAcquireHook(const char *format, ...);

unsigned char *__stdcall LoadResourceFile(const char *filename, unsigned int *size, int fromDisk)
{
    DWORD length;
    HANDLE file;
    unsigned char *data;
    if (g_MainRuntime.flags & 0x8000)
    {
        EnterCriticalSection(&g_MainRuntime.criticalSections[2]);
        ++g_MainRuntime.criticalDepth[2];
    }

    if (!fromDisk)
    {
        const char *base = strrchr(filename, '\\');
        if (!base)
            base = filename;
        else
            ++base;
        const char *name = strrchr(base, '/');
        if (!name)
            name = filename;
        else
            ++name;

        ArchiveEntryView *entry = g_GameArchive.entries;
        if (entry)
            for (int remaining = g_GameArchive.entryCount; remaining > 0; --remaining, ++entry)
                if (_stricmp(name, entry->name) == 0)
                {
                    length = entry->size;
                    goto have_length;
                }
        length = 0;
    have_length:
        if (size)
            *size = length;
        if (!length)
            goto archive_failed;
        InputAcquireHook("%s Decode ... \r\n", name);
        data = static_cast<unsigned char *>(malloc(length));
        if (!data)
            goto archive_failed;
        ReadArchivedFile(&g_GameArchive, name, data);
        LeaveRuntimeCriticalSection(&g_MainRuntime, 2);
        return data;
    }
    InputAcquireHook("%s Load ... \r\n", filename);
    file = CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, 0,
                       OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, 0);
    if (file == INVALID_HANDLE_VALUE)
    {
        InputAcquireHook("error : %s is not found.\r\n", filename);
        goto failed;
    }
    length = GetFileSize(file, 0);
    data = static_cast<unsigned char *>(malloc(length));
    if (!data)
    {
        InputAcquireHook("error : %s allocation error.\r\n", filename);
        CloseHandle(file);
    archive_failed:
    failed:
        UnlockSharedFile();
        return 0;
    }
    ReadFile(file, data, length, &length, 0);
    if (size)
        *size = length;
    CloseHandle(file);
    UnlockSharedFile();
    return data;
}
