#include "SharedFile.hpp"

// Same release-disabled variadic sink already declared by InputBackend.hpp.
void __cdecl InputAcquireHook(const char *format, ...);

int __stdcall OpenSharedFileForWrite(const char *filename)
{
    if (g_MainRuntime.flags & 0x8000)
    {
        EnterCriticalSection(&g_MainRuntime.criticalSections[2]);
        ++g_MainRuntime.criticalDepth[2];
    }

    g_SharedFile = CreateFileA(filename, GENERIC_WRITE, FILE_SHARE_READ, 0,
                               CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    if (g_SharedFile == INVALID_HANDLE_VALUE)
    {
        char *message;
        FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
                           FORMAT_MESSAGE_IGNORE_INSERTS,
                       0, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                       reinterpret_cast<char *>(&message), 0, 0);
        InputAcquireHook("error : %s write error %s\r\n", filename, message);
        LocalFree(message);
        UnlockSharedFile();
        return -1;
    }

    InputAcquireHook("%s open ...\r\n", filename);
    return 0;
}

int __stdcall OpenSharedFileForRead(const char *filename)
{
    if (g_MainRuntime.flags & 0x8000)
    {
        EnterCriticalSection(&g_MainRuntime.criticalSections[2]);
        ++g_MainRuntime.criticalDepth[2];
    }

    g_SharedFile = CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, 0,
                               OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, 0);
    if (g_SharedFile == INVALID_HANDLE_VALUE)
    {
        char *message;
        FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
                           FORMAT_MESSAGE_IGNORE_INSERTS,
                       0, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                       reinterpret_cast<char *>(&message), 0, 0);
        InputAcquireHook("error : %s write error %s\r\n", filename, message);
        LocalFree(message);
        UnlockSharedFile();
        return -1;
    }

    InputAcquireHook("%s open ...\r\n", filename);
    return 0;
}

int __stdcall WriteSharedFile(const void *buffer, unsigned int size)
{
    if (g_SharedFile == INVALID_HANDLE_VALUE)
        return -1;

    DWORD written;
    WriteFile(g_SharedFile, buffer, size, &written, 0);
    if (size != written)
    {
        CloseHandle(g_SharedFile);
        UnlockSharedFile();
        return -2;
    }
    return 0;
}
