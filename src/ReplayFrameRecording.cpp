#include "ReplayFrameRecording.hpp"
#include "InputBackend.hpp"
#include "MainRuntime.hpp"
#include "SecondaryReplayState.hpp"


// The native addresses 0x4A8E88 / 0x4A8D80 are independently observed; the
// original private class names of the gate and sampled player are not known.
extern void *g_ReplayRecordingGate;
extern ReplayFramePlayerSampleView *g_ReplayFramePlayerSample;

namespace
{
inline void AddReplayFrameEvent(ReplayFrameSegment *segment, unsigned short current,
    unsigned short pressed, unsigned short released)
{
    reinterpret_cast<unsigned short *>(segment->eventEnd)[0] = current;
    reinterpret_cast<unsigned short *>(segment->eventEnd)[1] = pressed;
    reinterpret_cast<unsigned short *>(segment->eventEnd)[2] = released;
    segment->eventEnd += 6;
}
}

// Complete 0x435FE0..0x436180: input processing plus 30-frame sample and
// 6-byte frame events. The callback's private EAX receiver is compiled from
// a natural callable C++ pointer rather than fabricated register bindings.
int __fastcall RecordReplayInputFrame(ReplayFrameRecordingView *self)
{
    if (!g_ReplayRecordingGate)
        return 1;

    const unsigned int mode = *reinterpret_cast<unsigned int *>(
        reinterpret_cast<unsigned char *>(&g_MainRuntime) + 0x200);
    ReplayInputRepeatView *input =
        reinterpret_cast<ReplayInputRepeatView *>(&g_InputState);
    input->previous = input->current;
    input->current = static_cast<unsigned short>(g_InputState.current);
    if (mode & 0x200)
    {
        if (input->current & 8) input->current |= 0x400;
        if (input->current & 1)
        {
            ++input->unknown114;
            if (input->unknown114 >= 8)
            {
                input->current |= 8;
                input->unknown114 = 8;
            }
        }
        else input->unknown114 = 0;
    }
    else if ((input->current & 1) && input->heldFrames[0] < 5 &&
             (input->current & 8) && input->heldFrames[3] < 5)
        input->current |= 0x400;

    UpdateReplayInputRepeatState(input);

    if (self->frame % 30 == 0)
    {
        const double rounded = static_cast<double>(g_ReplayFramePlayerSample->sample) + 0.5;
        const int value = rounded >= 256.0 ? 0xff :
            static_cast<unsigned char>(static_cast<int>(rounded));
        ReplayFrameSegment *segment = self->last->segment;
        *segment->sampleEnd++ = static_cast<unsigned char>(value);
    }

    ReplayFrameSegment *segment = self->last->segment;
    AddReplayFrameEvent(segment, static_cast<unsigned short>(input->current),
        static_cast<unsigned short>(input->pressed), static_cast<unsigned short>(input->released));
    if ((segment->eventEnd - reinterpret_cast<unsigned char *>(segment)) / 6 >= 900)
    {
        ReplayWriteSegmentNode *next = AppendReplayWriteSegment(
            reinterpret_cast<ReplayStateView *>(self), self->stage);
        self->last = reinterpret_cast<ReplayFrameSegmentNode *>(next);
    }
    ++self->frame;
    return 1;
}
