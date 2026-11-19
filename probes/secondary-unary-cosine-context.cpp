float __stdcall SecondaryUnaryMathB(float value);

extern "C" float __stdcall SecondaryUnaryCosineProbe(float angle)
{
    return SecondaryUnaryMathB(angle);
}
