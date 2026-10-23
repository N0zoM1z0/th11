#pragma once

struct InputStateView
{
    unsigned int current;
    unsigned int previous;
    unsigned int repeat;
    unsigned int pressed;
    unsigned int released;
    unsigned int heldFrames[32];
    unsigned char unknown094[0x98];
    unsigned int held;
};

typedef char InputStateViewSize[(sizeof(InputStateView) == 0x130) ? 1 : -1];

void __fastcall UpdateInputRepeatState(InputStateView *self);
