#pragma once

// Minimal physical timer header view used only by this neighboring comparison.
// The original C++ class/source unit is not identified.
struct StageTimerComparisonView
{
    int previous;
    int current;
    int IsAtOrBefore(int frame) const;
};
