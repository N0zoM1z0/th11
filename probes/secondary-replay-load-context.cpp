#include "../src/SecondaryReplayState.hpp"
#include "../src/TitleMenuRecord.hpp"
#include "../src/ResourceFile.hpp"
extern int __stdcall LoadReplayStageData(ReplayInitStateView *, void *);
TitleMenuRecordView *__stdcall CreateReplayPlayback(const char *filename);

// Non-runnable link context: the playback factory and a direct reload.
extern "C" int __stdcall SecondaryReplayLoadProbe(ReplayInitStateView *state, void *source)
{
    if (!CreateReplayPlayback(static_cast<const char *>(source)))
        return -1;
    unsigned int size;
    LoadResourceFile(static_cast<const char *>(source), &size, state->mode);
    return LoadReplayStageData(state, source);
}
