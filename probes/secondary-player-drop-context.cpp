#include "../src/SecondaryPlayerDrop.hpp"
extern "C" int __stdcall SecondaryPlayerDropProbe(SecondaryControllerDropManager *manager,
    const CameraVector *position, int kind, unsigned int color, float angle, float speed)
{
    return SpawnSecondaryPlayerDrop(manager, position, kind, color, angle, speed);
}
