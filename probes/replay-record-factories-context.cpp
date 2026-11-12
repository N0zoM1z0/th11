#include "ReplayRecordFactories.hpp"
extern "C" TitleMenuRecordView *__stdcall ReplayRecordFactoriesProbe(int kind, const char *name, int mode)
{
    if (kind) return CreateReplayPlayback(name);
    return CreateReplayRecording(mode);
}
