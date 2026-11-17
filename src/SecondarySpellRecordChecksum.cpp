#include "SecondarySpellStage.hpp"

// Original TH11 0x40BB50..0x40BB9F: validates the 32-bit obfuscated
// completion code at stage+0x90C. Keep C++ signed division and remainder:
// negative replay values must not silently become unsigned digits.
int __stdcall CheckSecondarySpellRecordCode(const SecondarySpellStageView *self)
{
    const int code = self->recordCode;
    const int hundredGroup = ((code / 100) % 100 + 34) % 100;
    const int lowGroup = (code % 100 + 67) % 100;
    return code / 100000 - 22 != hundredGroup + lowGroup;
}
