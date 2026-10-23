#pragma once

struct LogBufferView
{
    char text[8192];
    char *end;
};

typedef char LogBufferViewSize[(sizeof(LogBufferView) == 0x2004) ? 1 : -1];

const char *__cdecl AppendLog(LogBufferView *self, const char *format, ...);
