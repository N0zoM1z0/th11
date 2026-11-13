#include "PbgProcessLauncher.hpp"
#include <windows.h>

// Native 0x442E40..0x442F16. The startup struct is intentionally initialized
// field by field: the target leaves unrelated count/fill members at defaults
// and does not close the created process/thread handles in this function.
int __stdcall LaunchPbgChildProcess(const char *application, char *commandLine,
                                    int waitForExit)
{
    STARTUPINFOA startup;
    PROCESS_INFORMATION process;
    startup.dwX = 0x80000000u;
    startup.dwY = 0x80000000u;
    startup.dwXSize = 0x80000000u;
    startup.dwYSize = 0x80000000u;
    startup.wShowWindow = 10;
    startup.cbReserved2 = 0;
    startup.cb = sizeof(startup);
    startup.lpReserved = 0;
    startup.lpDesktop = 0;
    startup.lpTitle = 0;
    startup.dwXCountChars = 0x50;
    startup.dwYCountChars = 0x19;
    startup.dwFillAttribute = 0;
    startup.dwFlags = 0;
    startup.lpReserved2 = 0;
    startup.hStdInput = 0;
    startup.hStdOutput = 0;
    startup.hStdError = 0;

    if (!CreateProcessA(application, commandLine, 0, 0, FALSE,
                        NORMAL_PRIORITY_CLASS, 0, 0, &startup, &process))
        return -1;
    if (waitForExit)
    {
        DWORD exitCode = STILL_ACTIVE;
        do { GetExitCodeProcess(process.hProcess, &exitCode); }
        while (exitCode == STILL_ACTIVE);
        return static_cast<int>(exitCode);
    }
    return 0;
}
