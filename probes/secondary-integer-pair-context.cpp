#include "SecondaryIntegerPair.hpp"

extern "C" int __stdcall SecondaryIntegerPairProbe(SecondaryIntegerPair *left,
    const SecondaryIntegerPair *right, SecondaryIntegerPair *output, CameraVector *position,
    int factor, int x, int y)
{
    ConvertSecondaryIntegerPair(position, right);
    AddAssignSecondaryIntegerPair(output, left, right);
    int result = SecondaryIntegerPairNonzero(right);
    SubtractSecondaryIntegerPair(output, left, right);
    ScaleSecondaryIntegerPair(output, right, factor);
    DivideSecondaryIntegerPair100(output, right);
    SetSecondaryIntegerPair(output, x, y);
    AddSecondaryIntegerPair(output, left, right);
    AssignSecondaryIntegerPair(output, left, right);
    return result + output->x;
}
