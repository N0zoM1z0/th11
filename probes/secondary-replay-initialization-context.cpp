#include "../src/ReplayRecordFactories.hpp"

// Non-runnable link context: the recording and playback factories own the
// registry calls.
extern "C" int __stdcall SecondaryReplayInitializationProbe(int mode, const char *filename)
{
    TitleMenuRecordView *recording = CreateReplayRecording(mode);
    TitleMenuRecordView *playback = CreateReplayPlayback(filename);
    return (recording != 0) + (playback != 0);
}
