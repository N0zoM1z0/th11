#include "SecondaryActorRuntime.hpp"

void __stdcall AddSecondaryActorScore(int value)
{
    g_SecondaryActorScore = static_cast<int>(static_cast<unsigned int>(g_SecondaryActorScore) + value / 10);
    if (g_SecondaryActorScore >= 1000000000) g_SecondaryActorScore = 999999999;
}

void __stdcall IncrementSecondaryCollisionCount()
{
    if (g_SecondaryCollisionCount < 99999999) ++g_SecondaryCollisionCount;
}

void __stdcall AddSecondaryCollisionValue(int value)
{
    int result = static_cast<int>(static_cast<unsigned int>(g_SecondaryCollisionValue) + value);
    g_SecondaryCollisionValue = result;
    if (result > 13000) g_SecondaryCollisionValue = 13000;
    if (!(g_SecondaryCollisionTimer.flags & 1))
    {
        g_SecondaryCollisionTimer.flags |= 1;
        g_SecondaryCollisionTimer.scale = &g_AnimationTimeScale;
    }
    g_SecondaryCollisionTimer.current = 10;
    g_SecondaryCollisionTimer.fraction = 10.0f;
    g_SecondaryCollisionTimer.previous = 9;
}
