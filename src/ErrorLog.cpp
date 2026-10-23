#include "ErrorLog.hpp"
#include "MainRuntime.hpp"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

const char *__cdecl AppendErrorLog(ErrorLogBufferView *self, const char *format, ...)
{
    char buffer[512];
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
    if (self->buffer.end + length < self->buffer.text + 8191)
    {
        strcpy(self->buffer.end, buffer);
        self->buffer.end += length;
        *self->buffer.end = '\0';
    }
    self->hasError = 1;
    if (g_MainRuntime.flags & 0x8000)
    {
        LeaveCriticalSection(&g_MainRuntime.criticalSections[3]);
        --g_MainRuntime.criticalDepth[3];
    }
    return format;
}
