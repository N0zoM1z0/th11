#include "StageResource.hpp"

int __stdcall OffsetSecondaryTimer(StageTimerView *time, float amount);

// Negative frame offsets are kept separate from the positive adapter because
// the native owner negates the integer before converting it to x87 float.
struct StageTimerNegativeOffsetView : StageTimerView
{
    int OffsetByNegativeFrames(int frames);
};

int StageTimerNegativeOffsetView::OffsetByNegativeFrames(int frames)
{
    int negativeFrames = -frames;
    return OffsetSecondaryTimer(this, static_cast<float>(negativeFrames));
}
