#include "AsciiFormatting.hpp"
#include <stdarg.h>
#include <stdio.h>

void __cdecl AsciiManagerPrint(AsciiManagerView *self, const float *position,
    const char *format, ...)
{
    char text[256];
    va_list args;
    va_start(args, format);
    vsprintf(text, format, args);
    va_end(args);
    AsciiManagerAppendString(self, position, text);
}

void __cdecl AsciiManagerPrintSmall(AsciiManagerView *self, const float *position,
    const char *format, ...)
{
    char text[256];
    va_list args;
    va_start(args, format);
    vsprintf(text, format, args);
    va_end(args);
    AsciiManagerAppendString(self, position, text);
    self->strings[self->stringCount - 1].unknown120 = 1;
}
