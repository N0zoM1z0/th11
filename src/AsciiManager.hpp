#pragma once

// Partial TH11-local layout view. Unknown owners remain opaque storage.
// 0x004014E0 appends at most 320 records of 0x130 bytes. Its stores identify
// text[256], position, color, scale, and the lifetime field at +0x12C.
struct AsciiStringRecord
{
    char text[256];
    float position[3];
    unsigned int color;
    float scale[2];
    unsigned int unknown118;
    unsigned int unknown11c;
    unsigned int unknown120;
    unsigned int unknown124;
    unsigned int unknown128;
    int remainingFrames;
};

struct AsciiManagerView
{
    unsigned char unknown00000[0x87c];
    AsciiStringRecord strings[320];
    int stringCount;
    unsigned int color;
    float scale[2];
    unsigned int unknown1848c;
    unsigned int unknown18490;
    unsigned int unknown18494;
    unsigned int unknown18498;
    unsigned int unknown1849c;
    int stringLifetime;
    unsigned int unknown184a4;
    int frameCounter;
    unsigned char unknown184ac[0x10];
};

typedef char AsciiStringRecordSize[(sizeof(AsciiStringRecord) == 0x130) ? 1 : -1];
typedef char AsciiManagerViewSize[(sizeof(AsciiManagerView) == 0x184bc) ? 1 : -1];

void __stdcall AsciiManagerExpireStrings(AsciiManagerView *self);
int __fastcall AsciiManagerOnUpdate(AsciiManagerView *self);
