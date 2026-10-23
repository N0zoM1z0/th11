#include "StartupPath.hpp"
#include "FileSystem.hpp"
#include <shlobj.h>
#include <new>
#include <string.h>

int __stdcall ReadShortcutTarget(const char *filename, char *destination)
{
    int success = 0;
    if (destination)
    {
        CoInitialize(0);
        IShellLinkA *link;
        if (SUCCEEDED(CoCreateInstance(CLSID_ShellLink, 0, CLSCTX_INPROC_SERVER,
                                      IID_IShellLinkA, reinterpret_cast<void **>(&link))))
        {
            IPersistFile *file;
            if (SUCCEEDED(link->QueryInterface(IID_IPersistFile, reinterpret_cast<void **>(&file))))
            {
                WCHAR *wideFilename = new WCHAR[MAX_PATH];
                MultiByteToWideChar(CP_ACP, 0, filename, -1, wideFilename, MAX_PATH);
                if (SUCCEEDED(file->Load(wideFilename, STGM_READ)))
                {
                    WIN32_FIND_DATAA data;
                    if (SUCCEEDED(link->GetPath(destination, MAX_PATH, &data, 0)))
                        success = 1;
                }
                delete[] wideFilename;
                file->Release();
            }
            link->Release();
        }
        CoUninitialize();
    }
    return success;
}

int __cdecl CheckStartupPath()
{
    STARTUPINFOA startup = {sizeof(startup)};
    char modulePath[MAX_PATH + 1];
    char titlePath[MAX_PATH + 1];
    GetModuleFileNameA(0, modulePath, sizeof(modulePath));
    GetConsoleTitleA(titlePath, sizeof(titlePath));
    GetStartupInfoA(&startup);
    if (startup.lpTitle)
    {
        char *extension = strrchr(startup.lpTitle, '.');
        if (FileExists(startup.lpTitle) && extension)
        {
            if (!_stricmp(extension, g_ShortcutExtension))
            {
                do
                {
                    ReadShortcutTarget(startup.lpTitle, titlePath);
                    extension = strrchr(titlePath, '.');
                } while (!_stricmp(extension, g_ShortcutExtension));
            }
            else
                strcpy(titlePath, startup.lpTitle);
            if (strcmp(modulePath, titlePath))
                *(reinterpret_cast<unsigned char *>(&g_MainRuntime) + 0xb30) = 1;
        }
        g_MainRuntime.flags &= ~0x40u;
    }
    else
        g_MainRuntime.flags |= 0x40;
    return g_StartupReturnGate ? 0 : -1;
}
