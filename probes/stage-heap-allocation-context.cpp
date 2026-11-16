#include <stddef.h>
void *__stdcall AllocateStageHeapBytes(size_t size);

extern "C" __declspec(dllexport) void *__stdcall StageHeapAllocationProbe(size_t size)
{
    return AllocateStageHeapBytes(size);
}
