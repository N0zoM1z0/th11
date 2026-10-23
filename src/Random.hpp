#pragma once

struct RandomStateView
{
    unsigned short seed;
    unsigned short unknown02;
    unsigned int calls;
};

typedef char RandomStateViewSize[(sizeof(RandomStateView) == 8) ? 1 : -1];

unsigned short __stdcall RandomNext16(RandomStateView *self);
unsigned int __stdcall RandomNext32(RandomStateView *self);
