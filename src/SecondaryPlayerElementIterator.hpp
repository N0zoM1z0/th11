#pragma once

// The original 0x4021A0 iterator invokes an ECX-argument constructor
// through an indirect pointer. A callback may return the constructed
// element; the iterator itself disregards that value.
typedef void *(__fastcall *SecondaryElementConstructor)(void *element);
void __stdcall ConstructSecondaryPlayerElementArray(
    void *array, int stride, int count, SecondaryElementConstructor construct);
