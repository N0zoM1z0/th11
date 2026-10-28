#pragma once

// A value object preserves the observed by-value and hidden-result ABI.
struct AnimationVmId
{
    unsigned int value;

    AnimationVmId(unsigned int id) : value(id) {}
};

typedef char AnimationVmIdSize[(sizeof(AnimationVmId) == 4) ? 1 : -1];
