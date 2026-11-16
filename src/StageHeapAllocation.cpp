#include <stdlib.h>

// Compact native allocation facade at 0x00405D80: one count, one CRT malloc.
// The real caller's private LTCG ABI supplies the byte count in EAX.
void *__stdcall AllocateStageHeapBytes(size_t size)
{
    return malloc(size);
}
