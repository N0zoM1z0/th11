#pragma once
#include "Camera.hpp"

// Physical two-component integer view; original vector class/return ABI is open.
struct SecondaryIntegerPair
{
    int x, y;
};

CameraVector *__stdcall ConvertSecondaryIntegerPair(CameraVector *output, const SecondaryIntegerPair *input);
SecondaryIntegerPair *__stdcall AddAssignSecondaryIntegerPair(SecondaryIntegerPair *output,
    SecondaryIntegerPair *left, const SecondaryIntegerPair *right);
int __stdcall SecondaryIntegerPairNonzero(const SecondaryIntegerPair *value);
SecondaryIntegerPair *__stdcall SubtractSecondaryIntegerPair(SecondaryIntegerPair *output,
    const SecondaryIntegerPair *left, const SecondaryIntegerPair *right);
SecondaryIntegerPair *__stdcall ScaleSecondaryIntegerPair(SecondaryIntegerPair *output,
    const SecondaryIntegerPair *input, int factor);
SecondaryIntegerPair *__stdcall DivideSecondaryIntegerPair100(SecondaryIntegerPair *output,
    const SecondaryIntegerPair *input);
SecondaryIntegerPair *__stdcall SetSecondaryIntegerPair(SecondaryIntegerPair *output, int x, int y);
SecondaryIntegerPair *__stdcall AddSecondaryIntegerPair(SecondaryIntegerPair *output,
    const SecondaryIntegerPair *left, const SecondaryIntegerPair *right);
SecondaryIntegerPair *__stdcall AssignSecondaryIntegerPair(SecondaryIntegerPair *output,
    SecondaryIntegerPair *left, const SecondaryIntegerPair *right);

typedef char SecondaryIntegerPairSize[(sizeof(SecondaryIntegerPair) == 8) ? 1 : -1];
