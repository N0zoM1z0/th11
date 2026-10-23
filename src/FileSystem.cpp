#include "FileSystem.hpp"
#include <windows.h>

extern unsigned int g_InputRuntimeFlags;
extern CRITICAL_SECTION g_FileCriticalSection;
extern unsigned char g_FileCriticalDepth;

int __stdcall FileExists(const char *path)
{
    if (g_InputRuntimeFlags & 0x8000)
    {
        EnterCriticalSection(&g_FileCriticalSection);
        ++g_FileCriticalDepth;
    }

    HANDLE file = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, NULL,
                             OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    if (file != INVALID_HANDLE_VALUE)
    {
        CloseHandle(file);
        if (g_InputRuntimeFlags & 0x8000)
        {
            LeaveCriticalSection(&g_FileCriticalSection);
            --g_FileCriticalDepth;
        }
        return 1;
    }

    if (g_InputRuntimeFlags & 0x8000)
    {
        LeaveCriticalSection(&g_FileCriticalSection);
        --g_FileCriticalDepth;
    }
    return 0;
}
