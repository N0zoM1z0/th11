#include "ByteCipher.hpp"
#include <stdlib.h>
#include <string.h>

// TH11 0x004581C0..0x004582DB. A short remainder and an odd trailing byte
// are left alone. The caller supplies a positive block size and a copy limit
// that contains every processed block; the target adds no bounds checks.
unsigned char * __stdcall ScatterXorBlocks(unsigned char *data, int size,
    unsigned char key, unsigned char step, int blockSize, int limit)
{
    const int remainder = size % blockSize;
    const int tail = remainder < blockSize / 4 ? remainder : 0;
    const int copySize = size < limit ? size : limit;
    unsigned char *temporary = static_cast<unsigned char *>(malloc(copySize));
    if (temporary)
    {
        size -= (size & 1) + tail;
        memcpy(temporary, data, copySize);
        unsigned char *input = temporary;
        unsigned char *output = data;
        while (size > 0 && limit > 0)
        {
            if (size < blockSize)
                blockSize = size;
            output += blockSize;
            unsigned char *position = output - 1;
            for (int count = (blockSize + 1) / 2; count > 0; --count)
            {
                *position = *input++ ^ key;
                key += step;
                position -= 2;
            }
            position = output - 2;
            for (int count = blockSize / 2; count > 0; --count)
            {
                *position = *input++ ^ key;
                key += step;
                position -= 2;
            }
            size -= blockSize;
            limit -= blockSize;
        }
        free(temporary);
    }
    return data;
}

// TH11 0x004582E0..0x004583FB reads the same interleaving into consecutive
// output bytes. Allocation failure returns the original pointer unchanged.
unsigned char * __stdcall GatherXorBlocks(unsigned char *data, int size,
    unsigned char key, unsigned char step, int blockSize, int limit)
{
    const int remainder = size % blockSize;
    const int tail = remainder < blockSize / 4 ? remainder : 0;
    const int copySize = size < limit ? size : limit;
    unsigned char *temporary = static_cast<unsigned char *>(malloc(copySize));
    if (temporary)
    {
        size -= (size & 1) + tail;
        memcpy(temporary, data, copySize);
        unsigned char *input = temporary;
        unsigned char *output = data;
        while (size > 0 && limit > 0)
        {
            if (size < blockSize)
                blockSize = size;
            input += blockSize;
            unsigned char *position = input - 1;
            for (int count = (blockSize + 1) / 2; count > 0; --count)
            {
                *output++ = *position ^ key;
                key += step;
                position -= 2;
            }
            position = input - 2;
            for (int count = blockSize / 2; count > 0; --count)
            {
                *output++ = *position ^ key;
                key += step;
                position -= 2;
            }
            size -= blockSize;
            limit -= blockSize;
        }
        free(temporary);
    }
    return data;
}
