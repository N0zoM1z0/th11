#include "ExecutableChecksum.hpp"
#include "ResourceFile.hpp"
#include <stdlib.h>

unsigned int __cdecl ComputeExecutableChecksum()
{
    char filename[MAX_PATH + 1];
    if (GetModuleFileNameA(0, filename, sizeof(filename)))
    {
        unsigned int size;
        unsigned char *buffer = LoadResourceFile(filename, &size, 1);
        if (!buffer)
            return ~0u;
        unsigned int checksum = 0;
        unsigned int *cursor = reinterpret_cast<unsigned int *>(buffer);
        for (int i = 0; i < static_cast<int>(size) / 4 - 1; ++i)
            checksum += *cursor++;
        free(buffer);
        ExecutableChecksumView &state = *reinterpret_cast<ExecutableChecksumView *>(
            reinterpret_cast<unsigned char *>(&g_MainRuntime) + 0x998);
        state.size = size;
        state.checksum = checksum;
        return checksum;
    }
    return ~0u;
}
