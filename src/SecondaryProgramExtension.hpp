#pragma once
#include "SecondaryScript.hpp"

// Observed virtual and allocation layout; unknown storage retains no guessed ownership.
struct SecondaryProgramWithExtensions : SecondaryProgramView
{
    SecondaryProgramWithExtensions();
    virtual int ProcessExtension(void *data);
    virtual int LoadFile(const char *filename);
};
typedef char SecondaryProgramDerivedSize[(sizeof(SecondaryProgramWithExtensions) == 0x1098) ? 1 : -1];
