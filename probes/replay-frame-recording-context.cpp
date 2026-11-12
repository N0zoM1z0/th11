#include "ReplayFrameRecording.hpp"
// Separate natural callback caller; owned game threading, VM and physical
// replay storage remain elsewhere.
extern "C" int __stdcall ReplayFrameRecordingProbe(ReplayFrameRecordingView *self)
{
    return RecordReplayInputFrame(self);
}
