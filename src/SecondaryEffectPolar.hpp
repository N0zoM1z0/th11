#pragma once

// Exactly two floats of output for the native 0x419BB0 elliptical polar
// helper. The original class identity is not established by the protocol.
struct SecondaryEffectPolarVector
{
    float x, y;
    void SetPolar(float angle, float width, float height);
};
