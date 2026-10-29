#pragma once
#include "SecondaryScript.hpp"

// Only the observed virtual prefix; construction and the full derived layout remain open.
struct SecondaryProgramWithExtensions : SecondaryProgramView
{
    virtual int ProcessExtension(void *data);
    virtual int LoadFile(const char *filename);
};
