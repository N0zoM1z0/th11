#include "WholeFile.hpp"
#include "SharedFile.hpp"

void __cdecl InputAcquireHook(const char *format, ...);

int __stdcall WriteWholeFile(const char *filename, const void *buffer, unsigned int size)
{
    if (g_MainRuntime.flags & 0x8000)
    {
        EnterCriticalSection(&g_MainRuntime.criticalSections[2]);
        ++g_MainRuntime.criticalDepth[2];
    }

    HANDLE file = CreateFileA(filename, GENERIC_WRITE, FILE_SHARE_READ, 0,
                              CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    if (file == INVALID_HANDLE_VALUE)
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

    DWORD written;
    WriteFile(file, buffer, size, &written, 0);
    if (size != written)
    {
        CloseHandle(file);
        InputAcquireHook("error : %s write error\r\n", filename);
        UnlockSharedFile();
        return -2;
    }

    CloseHandle(file);
    InputAcquireHook("%s write ...\r\n", filename);
    UnlockSharedFile();
    return 0;
}
