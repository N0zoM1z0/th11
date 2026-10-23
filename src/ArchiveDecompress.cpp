#include "Archive.hpp"

#include <stdlib.h>

// The target resets the ring cursor per call, but retains this shared storage.
unsigned char g_ArchiveHistory[8192];

static __forceinline int ArchiveReadBit(unsigned char *data, int size,
    unsigned char *&cursor, int &cached, unsigned char &mask)
{
    if (mask == 0x80)
    {
        // The native reader fetches before testing the signed packed length.
        cached = *cursor;
        if (cursor - data >= size)
            cached = 0;
        else
            ++cursor;
    }
    int result = mask & cached;
    mask >>= 1;
    if (!mask)
        mask = 0x80;
    return result;
}

static __forceinline int ArchiveReadBits(int count, unsigned char *data, int size,
    unsigned char *&cursor, int &cached, unsigned char &mask)
{
    int result = 0;
    unsigned int bit = 1u << (count - 1);
    while (bit != 0)
    {
        if (mask == 0x80)
        {
            cached = *cursor;
            if (cursor - data >= size)
                cached = 0;
            else
                ++cursor;
        }
        if (cached & mask)
            result |= bit;
        bit >>= 1;
        mask >>= 1;
        if (!mask)
            mask = 0x80;
    }
    return result;
}

unsigned char *__stdcall DecompressArchiveData(unsigned int outputSize,
                                               unsigned char *data, int size,
                                               unsigned char *destination)
{
    unsigned char mask = 0x80;
    int cached = 0;
    if (!destination)
    {
        destination = static_cast<unsigned char *>(malloc(outputSize));
        if (!destination)
            return 0;
    }

    unsigned char *cursor = data;
    unsigned char *output = destination;
    int position = 1;
    for (;;)
    {
        if (ArchiveReadBit(data, size, cursor, cached, mask))
        {
            unsigned char value = ArchiveReadBits(8, data, size, cursor, cached, mask);
            *output++ = value;
            g_ArchiveHistory[position] = value;
            position = (position + 1) & 0x1fff;
        }
        else
        {
            int reference = ArchiveReadBits(13, data, size, cursor, cached, mask);
            if (!reference)
                break;
            int length = ArchiveReadBits(4, data, size, cursor, cached, mask) + 2;
            for (int index = 0; index <= length; ++index)
            {
                unsigned char value = g_ArchiveHistory[(reference + index) & 0x1fff];
                *output++ = value;
                g_ArchiveHistory[position] = value;
                position = (position + 1) & 0x1fff;
            }
        }
    }
    while (mask != 0x80)
        ArchiveReadBit(data, size, cursor, cached, mask);
    return destination;
}
