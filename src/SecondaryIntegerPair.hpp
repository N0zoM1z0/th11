#pragma once
#include "Camera.hpp"

// Fixed-point (1/128) integer pair. Native keeps dead out-of-line copies of
// these inline operators at 433DD0..433EB1 after all their uses were inlined.
struct SecondaryIntegerPair
{
    int x, y;

    SecondaryIntegerPair operator+=(const SecondaryIntegerPair &other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }
    SecondaryIntegerPair operator-(const SecondaryIntegerPair &other) const
    {
        SecondaryIntegerPair result;
        result.x = x - other.x;
        result.y = y - other.y;
        return result;
    }
    SecondaryIntegerPair operator*(int factor) const
    {
        SecondaryIntegerPair result;
        result.x = x * factor;
        result.y = y * factor;
        return result;
    }
    SecondaryIntegerPair operator/(int divisor) const
    {
        SecondaryIntegerPair result;
        result.x = x / divisor;
        result.y = y / divisor;
        return result;
    }
    SecondaryIntegerPair operator+(const SecondaryIntegerPair &other) const
    {
        SecondaryIntegerPair result;
        result.x = x + other.x;
        result.y = y + other.y;
        return result;
    }
};

CameraVector *__stdcall ConvertSecondaryIntegerPair(CameraVector *output, const SecondaryIntegerPair *input);
int __stdcall SecondaryIntegerPairNonzero(const SecondaryIntegerPair *value);
SecondaryIntegerPair *__stdcall SetSecondaryIntegerPair(SecondaryIntegerPair *output, int x, int y);
SecondaryIntegerPair *__stdcall AssignSecondaryIntegerPair(SecondaryIntegerPair *output,
    SecondaryIntegerPair *left, const SecondaryIntegerPair *right);

typedef char SecondaryIntegerPairSize[(sizeof(SecondaryIntegerPair) == 8) ? 1 : -1];
