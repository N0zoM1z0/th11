#include "ReplayPayloadChecksum.hpp"

// 0x437500: byte sum of the payload after the 8-byte header. VC9 splits the
// loop into two accumulators and an odd trailing byte.
unsigned int ReplayPayloadChecksumView::SumPayloadBytes(int length)
{
    unsigned int sum = 0;
    for (int i = 0; i < length - 8; ++i)
        sum += bytes[i];
    return sum;
}
