#include "ReplayPayloadChecksum.hpp"

// Original 0x437500..0x43754E: two interleaved byte accumulators and a
// possible odd trailing byte. The early tail lifetime naturally recovers the
// native stack spill without instructions, explicit register bindings or
// fabricated padding.
unsigned int ReplayPayloadChecksumView::SumPayloadBytes(int length)
{
    unsigned int tail = 0;
    unsigned int sumEven = 0, sumOdd = 0;
    int offset = 0;
    int last = length - 8;
    if (last >= 2)
    {
        do
        {
            sumEven += bytes[offset];
            sumOdd += bytes[offset + 1];
            offset += 2;
        } while (offset < last - 1);
    }
    if (offset < last)
        tail = bytes[offset];
    return sumOdd + sumEven + tail;
}
