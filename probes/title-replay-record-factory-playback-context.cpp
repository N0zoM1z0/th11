#include "TitleMenuRecord.hpp"
TitleMenuRecordView *__stdcall CreateReplayPlayback(const char *filename);
extern "C" TitleMenuRecordView *__stdcall TitleReplayRecordFactoryPlaybackProbe(const char *filename)
{
    return CreateReplayPlayback(filename);
}
