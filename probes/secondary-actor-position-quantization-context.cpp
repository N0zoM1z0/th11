extern void __stdcall QuantizeSecondaryActorPosition(const float *position);
extern "C" void __stdcall SecondaryActorPositionQuantizationProbe(const float *position)
{
    QuantizeSecondaryActorPosition(position);
}
