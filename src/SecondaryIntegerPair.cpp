#include "SecondaryIntegerPair.hpp"

CameraVector *__stdcall ConvertSecondaryIntegerPair(CameraVector *output, const SecondaryIntegerPair *input)
{
    output->x = static_cast<float>(input->x * 0.0078125);
    output->y = static_cast<float>(input->y * 0.0078125);
    output->z = 0.0f;
    return output;
}

SecondaryIntegerPair *__stdcall AddAssignSecondaryIntegerPair(SecondaryIntegerPair *output,
    SecondaryIntegerPair *left, const SecondaryIntegerPair *right)
{
    left->x = static_cast<int>(static_cast<unsigned int>(left->x) + right->x);
    left->y = static_cast<int>(static_cast<unsigned int>(left->y) + right->y);
    int x = left->x, y = left->y;
    output->x = x;
    output->y = y;
    return output;
}

int __stdcall SecondaryIntegerPairNonzero(const SecondaryIntegerPair *value)
{
    return value->x || value->y ? 1 : 0;
}

SecondaryIntegerPair *__stdcall SubtractSecondaryIntegerPair(SecondaryIntegerPair *output,
    const SecondaryIntegerPair *left, const SecondaryIntegerPair *right)
{
    int x = static_cast<int>(static_cast<unsigned int>(left->x) - right->x);
    int y = static_cast<int>(static_cast<unsigned int>(left->y) - right->y);
    output->x = x;
    output->y = y;
    return output;
}

SecondaryIntegerPair *__stdcall ScaleSecondaryIntegerPair(SecondaryIntegerPair *output,
    const SecondaryIntegerPair *input, int factor)
{
    int x = static_cast<int>(static_cast<unsigned int>(input->x) * factor);
    int y = static_cast<int>(static_cast<unsigned int>(input->y) * factor);
    output->x = x;
    output->y = y;
    return output;
}

SecondaryIntegerPair *__stdcall DivideSecondaryIntegerPair100(SecondaryIntegerPair *output,
    const SecondaryIntegerPair *input)
{
    int x = input->x, y = input->y;
    output->x = x / 100;
    output->y = y / 100;
    return output;
}

SecondaryIntegerPair *__stdcall SetSecondaryIntegerPair(SecondaryIntegerPair *output, int x, int y)
{
    output->x = x;
    output->y = y;
    return output;
}

SecondaryIntegerPair *__stdcall AddSecondaryIntegerPair(SecondaryIntegerPair *output,
    const SecondaryIntegerPair *left, const SecondaryIntegerPair *right)
{
    int x = static_cast<int>(static_cast<unsigned int>(left->x) + right->x);
    int y = static_cast<int>(static_cast<unsigned int>(left->y) + right->y);
    output->x = x;
    output->y = y;
    return output;
}

SecondaryIntegerPair *__stdcall AssignSecondaryIntegerPair(SecondaryIntegerPair *output,
    SecondaryIntegerPair *left, const SecondaryIntegerPair *right)
{
    left->x = right->x;
    left->y = right->y;
    int x = left->x, y = left->y;
    output->x = x;
    output->y = y;
    return output;
}
