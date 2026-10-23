#pragma once

#include "MainRuntime.hpp"

extern HANDLE g_SharedFile;

int __stdcall OpenSharedFileForWrite(const char *filename);
int __stdcall OpenSharedFileForRead(const char *filename);
int __stdcall WriteSharedFile(const void *buffer, unsigned int size);

inline void UnlockSharedFile()
{
    if (g_MainRuntime.flags & 0x8000)
    {
        LeaveCriticalSection(&g_MainRuntime.criticalSections[2]);
        --g_MainRuntime.criticalDepth[2];
    }
}

inline void CloseSharedFile()
{
    if (g_SharedFile != INVALID_HANDLE_VALUE)
    {
        CloseHandle(g_SharedFile);
        UnlockSharedFile();
    }
}
