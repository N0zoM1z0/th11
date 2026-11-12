#include "ReplayFrameRecording.hpp"
#include "InputBackend.hpp"
#include "MainRuntime.hpp"

struct ReplayWriteState;
struct ReplayWriteSegmentNode;
ReplayWriteSegmentNode *__stdcall AppendReplayWriteSegment(ReplayWriteState *state, int stage);

// The native addresses 0x4A8E88 / 0x4A8D80 are independently observed; the
// original private class names of the gate and sampled player are not known.
extern void *g_ReplayRecordingGate;
extern ReplayFramePlayerSampleView *g_ReplayFramePlayerSample;

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
        const unsigned char value = rounded >= 256.0 ? 0xff :
            static_cast<unsigned char>(static_cast<int>(rounded));
        ReplayFrameSegment *segment = self->last->segment;
        *segment->sampleEnd++ = value;
    }

    ReplayFrameSegment *segment = self->last->segment;
    unsigned short *write = reinterpret_cast<unsigned short *>(segment->eventEnd);
    write[0] = static_cast<unsigned short>(input->current);
    write[1] = static_cast<unsigned short>(input->pressed);
    write[2] = static_cast<unsigned short>(input->released);
    segment->eventEnd += 6;
    if ((segment->eventEnd - reinterpret_cast<unsigned char *>(segment)) / 6 >= 900)
        self->last = reinterpret_cast<ReplayFrameSegmentNode *>(
            AppendReplayWriteSegment(reinterpret_cast<ReplayWriteState *>(self), self->stage));
    ++self->frame;
    return 1;
}
