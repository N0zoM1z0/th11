float __stdcall SecondaryUnaryMathA(float value);

extern "C" float __stdcall SecondaryUnarySineProbe(float angle)
{
    return SecondaryUnaryMathA(angle);
}
