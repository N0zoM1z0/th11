#pragma once

#include "LogBuffer.hpp"
#include <stddef.h>

struct ErrorLogBufferView
{
    LogBufferView buffer;
    unsigned char hasError;
};

typedef char ErrorLogFlagOffset[(offsetof(ErrorLogBufferView, hasError) == 0x2004) ? 1 : -1];

const char *__cdecl AppendErrorLog(ErrorLogBufferView *self, const char *format, ...);
extern ErrorLogBufferView g_RuntimeLog;
