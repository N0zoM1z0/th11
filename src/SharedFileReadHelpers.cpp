#include "SharedFile.hpp"
#include <stdlib.h>

extern unsigned int g_InputRuntimeFlags;
extern CRITICAL_SECTION g_FileCriticalSection;
extern unsigned char g_FileCriticalDepth;

// Full 0x458980..0x4589C4: read a caller-sized buffer from the already-open
// global shared handle. Native LTCG passes the requested size in EDI.
unsigned char *__stdcall ReadSharedReplayBytes(unsigned int size)
{
    HANDLE file = g_SharedFile;
    if (file == INVALID_HANDLE_VALUE) return 0;
    unsigned char *buffer = static_cast<unsigned char *>(malloc(size));
    if (!buffer)
    {
        CloseHandle(file);
        return 0;
    }
    DWORD read;
    ReadFile(file, buffer, size, &read, 0);
    return buffer;
}

// Full 0x4589D0..0x458A00: close the shared read handle and conditionally
// leave the same runtime critical section entered by OpenSharedFileForRead.
int __cdecl CloseSharedReplayReader()
{
    if (g_SharedFile != INVALID_HANDLE_VALUE)
    {
        CloseHandle(g_SharedFile);
        if (g_InputRuntimeFlags & 0x8000)
        {
            LeaveCriticalSection(&g_FileCriticalSection);
            --g_FileCriticalDepth;
        }
    }
    return 0;
}
