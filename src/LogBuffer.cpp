#include "LogBuffer.hpp"
#include "MainRuntime.hpp"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

const char *__cdecl AppendLog(LogBufferView *self, const char *format, ...)
{
    char buffer[8192];
    if (g_MainRuntime.flags & 0x8000)
    {
        EnterCriticalSection(&g_MainRuntime.criticalSections[3]);
        ++g_MainRuntime.criticalDepth[3];
    }

    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);

    unsigned int length = strlen(buffer);
    if (self->end + length < self->text + 8191)
    {
        strcpy(self->end, buffer);
        self->end += length;
        *self->end = '\0';
    }

    if (g_MainRuntime.flags & 0x8000)
    {
        LeaveCriticalSection(&g_MainRuntime.criticalSections[3]);
        --g_MainRuntime.criticalDepth[3];
    }
    return format;
}
