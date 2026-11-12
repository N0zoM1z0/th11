#include "PbgLowLevelFile.hpp"
#include <string.h>

// Native 0x442000..0x442078 (121 bytes): retain absolute filenames as is;
// otherwise form the executable directory prefix plus the requested basename.
// Original private filename-in-EAX and destination-in-ECX are linked details.
void __fastcall ResolvePbgDiskPath(char *destination, const char *filename)
{
    if (strchr(filename, ':'))
    {
        strcpy(destination, filename);
        return;
    }
    GetModuleFileNameA(0, destination, MAX_PATH);
    char *slash = strrchr(destination, '\\');
    if (!slash)
        *destination = 0;
    // Preserve the original unguarded use of the last-separator pointer.
    slash[1] = 0;
    strcat(destination, filename);
}
