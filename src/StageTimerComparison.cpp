#include "StageTimerComparison.hpp"

int StageTimerComparisonView::IsAtOrBefore(int frame) const
{
    return current <= frame;
}
