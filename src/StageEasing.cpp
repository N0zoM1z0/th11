#include "StageTween.hpp"

float __stdcall EvaluateStageEasing(int curve, float progress)
{
    // The original dispatch has no bounds check; callers enforce this range.
    __assume(static_cast<unsigned int>(curve) < 16);
    switch (curve)
    {
    case 0:
        return progress * progress;
    case 3:
    {
        return static_cast<float>(1.0 - (1.0 - progress) * (1.0 - progress));
    }
    case 1:
        return progress * progress * progress;
    case 4:
    {
        return static_cast<float>(1.0 - (1.0 - progress) * (1.0 - progress) * (1.0 - progress));
    }
    case 2:
        return progress * progress * progress * progress;
    case 5:
    {
        return static_cast<float>(1.0 - (1.0 - progress) * (1.0 - progress) * (1.0 - progress) * (1.0 - progress));
    }
    case 8:
    {
        progress = static_cast<float>(progress * 2.0);
        if (progress < 1.0f)
            return static_cast<float>(progress * progress * 0.5);
        return static_cast<float>((2.0 - (2.0 - progress) * (2.0 - progress)) * 0.5);
    }
    case 11:
    {
        progress = progress + progress;
        if (progress < 1.0f)
        {
            return static_cast<float>(0.5 - (1.0 - progress) * (1.0 - progress) * 0.5);
        }
        return static_cast<float>((progress - 1.0) * (progress - 1.0) * 0.5 + 0.5);
    }
    case 9:
    {
        progress = static_cast<float>(progress * 2.0);
        if (progress < 1.0f)
            return static_cast<float>(progress * progress * progress * 0.5);
        return static_cast<float>((2.0 - (2.0 - progress) * (2.0 - progress) * (2.0 - progress)) * 0.5);
    }
    case 12:
    {
        progress = progress + progress;
        if (progress < 1.0f)
        {
            return static_cast<float>(0.5 - (1.0 - progress) * (1.0 - progress) * (1.0 - progress) * 0.5);
        }
        return static_cast<float>((progress - 1.0) * (progress - 1.0) * (progress - 1.0) * 0.5 + 0.5);
    }
    case 10:
    {
        progress = static_cast<float>(progress * 2.0);
        if (progress < 1.0f)
            return static_cast<float>(progress * progress * progress * progress * 0.5);
        return static_cast<float>((2.0 - (2.0 - progress) * (2.0 - progress) * (2.0 - progress) * (2.0 - progress)) * 0.5);
    }
    case 13:
    {
        progress = progress + progress;
        if (progress < 1.0f)
        {
            return static_cast<float>(0.5 - (1.0 - progress) * (1.0 - progress) * (1.0 - progress) * (1.0 - progress) * 0.5);
        }
        return static_cast<float>((progress - 1.0) * (progress - 1.0) * (progress - 1.0) * (progress - 1.0) * 0.5 + 0.5);
    }
    case 14: return 0.0f;
    case 15: return 1.0f;
    case 6:
    case 7: return progress;
    default: __assume(0);
    }
}
