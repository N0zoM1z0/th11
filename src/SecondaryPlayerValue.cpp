// item.cpp: 0x424640 adds to the game-state value at +0x10 (0x4A56F0) and
// clamps it to the maximum at +0x8C (0x4A576C). Item collection calls it with
// 10 and 1000.
extern int g_SecondaryPlayerValue, g_SecondaryPlayerValueMaximum;

void __stdcall AddSecondaryPlayerValue(int amount)
{
    g_SecondaryPlayerValue += amount;
    if (g_SecondaryPlayerValue > g_SecondaryPlayerValueMaximum)
        g_SecondaryPlayerValue = g_SecondaryPlayerValueMaximum;
}
