#pragma once
#include "AsciiAppend.hpp"

void __cdecl AsciiManagerPrint(AsciiManagerView *self, const float *position,
    const char *format, ...);
void __cdecl AsciiManagerPrintSmall(AsciiManagerView *self, const float *position,
    const char *format, ...);
