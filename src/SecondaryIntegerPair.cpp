#include "SecondaryIntegerPair.hpp"

CameraVector *__stdcall ConvertSecondaryIntegerPair(CameraVector *output, const SecondaryIntegerPair *input)
{
    output->x = static_cast<float>(input->x * 0.0078125);
    output->y = static_cast<float>(input->y * 0.0078125);
    output->z = 0.0f;
    return output;
}

int __stdcall SecondaryIntegerPairNonzero(const SecondaryIntegerPair *value)
{
    return value->x || value->y ? 1 : 0;
}

SecondaryIntegerPair *__stdcall SetSecondaryIntegerPair(SecondaryIntegerPair *output, int x, int y)
{
    output->x = x;
    output->y = y;
    return output;
}
