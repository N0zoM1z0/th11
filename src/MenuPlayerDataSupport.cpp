#include "MenuPlayerData.hpp"
#include "SecondaryActorRuntime.hpp"
#include <string.h>

int __stdcall CountMenuSpellsForDifficulty(int difficulty)
{
    int count = 0;
    for (int i = 0; i < 175; ++i)
        if (g_MenuSpellDifficulties[i] == difficulty) ++count;
    return count;
}

void __cdecl UnlockMenuPlayerDataEntries()
{
    unsigned char *statistics = g_SecondaryGameplayStatistics;
    *reinterpret_cast<unsigned int *>(statistics + 0x2ddea) = 0x11111111;
    *reinterpret_cast<unsigned int *>(statistics + 0x2ddee) = 0x11111111;
    *reinterpret_cast<unsigned int *>(statistics + 0x2ddf2) = 0x11111111;
    *reinterpret_cast<unsigned int *>(statistics + 0x2ddf6) = 0x11111111;
    unsigned char *character = statistics + 0x5b1;
    for (int i = 0; i < 6; ++i)
    {
        unsigned char *flag = character;
        for (int difficulty = 0; difficulty < 4; ++difficulty)
            for (int stage = 0; stage < 6; ++stage)
            {
                *flag = 1;
                flag += 8;
            }
        character += 0x68d4;
    }
}

int __stdcall ReadMenuKeyboardSnapshot(unsigned char *keys)
{
    memset(keys, 0, 256);
    if (g_KeyboardPollingEnabled)
    {
        if (!(g_InputRuntimeFlags & 0x400))
        {
            GetKeyboardState(keys);
            return 2;
        }
        HRESULT result = g_KeyboardDevice->vtable->GetDeviceState(g_KeyboardDevice, 256, keys);
        if (result == static_cast<HRESULT>(0x8007001e))
            g_KeyboardDevice->vtable->Acquire(g_KeyboardDevice);
        else if (result != 0)
            g_KeyboardDevice->vtable->Acquire(g_KeyboardDevice);
        return 1;
    }
    return 0;
}
