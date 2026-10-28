#include "StageUpdate.hpp"

extern ChainCallback g_StageUpdateProbeCallback;

extern "C" int __stdcall StageUpdateProbe(StageResourceView *first, StageResourceView *second)
{
    g_StageUpdateProbeCallback = StageUpdateCallback;
    int result = StageUpdateCallback(first);
    result += StageUpdateCallback(second);
    return result;
}
