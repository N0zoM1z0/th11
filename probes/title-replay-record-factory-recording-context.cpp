#include "TitleMenuRecord.hpp"
TitleMenuRecordView *__stdcall CreateReplayRecording(int mode);
extern "C" TitleMenuRecordView *__stdcall TitleReplayRecordFactoryRecordingProbe(int mode)
{
    return CreateReplayRecording(mode);
}
