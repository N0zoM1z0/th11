#pragma once
#include <stddef.h>

// game.cpp's "GameInf" object, published at 0x4A8E88 by 0x4202D0. The older
// SecondaryDialogueSessionView names the same pointer. The word at +0x60 is a
// bit field: the laser update callback tests bits 0 and 2 as
// (word | word >> 2) & 1, which VC9 emits for two one-bit members, not for a
// mask. Bit 1 runs updates with the animation time scale held at zero; bit 2
// also suppresses drawing.
struct GameInf
{
    unsigned char unknown000[0x14];
    int time014;
    unsigned char unknown018[0x60 - 0x18];
    unsigned int flag0 : 1;
    unsigned int freezeTime : 1;
    unsigned int flag2 : 1;
    unsigned int flag3 : 1;
    unsigned int flag4 : 1;
    unsigned int flag5 : 1;
    unsigned int flag6 : 1;
    unsigned int flag7 : 1;
    unsigned int flag8 : 1;
    unsigned int flag9 : 1;
    unsigned int flag10 : 1;
    unsigned int : 21;
    unsigned char unknown064[0x74 - 0x64];
    int mode;
};
typedef char GameInfMode[(offsetof(GameInf, mode) == 0x74) ? 1 : -1];
typedef char GameInfSize[(sizeof(GameInf) >= 0x78) ? 1 : -1];

extern GameInf *g_Game;
