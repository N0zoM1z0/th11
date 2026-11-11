#pragma once

// Physical 8-byte elements of the seven-pair arrays embedded in each
// player emitter. The original nominal C++ type identities remain open.
struct PlayerEmitterPolarPair
{
    float x, y;
    PlayerEmitterPolarPair();
};
struct PlayerEmitterOffsetPair
{
    int x, y;
    PlayerEmitterOffsetPair();
};

typedef char PlayerPolarPairSize[(sizeof(PlayerEmitterPolarPair) == 8) ? 1 : -1];
typedef char PlayerOffsetPairSize[(sizeof(PlayerEmitterOffsetPair) == 8) ? 1 : -1];
