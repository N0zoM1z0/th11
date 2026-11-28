#include "Laser.hpp"

// LaserDataInf default slots, 0x4246E0..0x424820. Native code order is
// slots 0-4, 7, 6, 8, 5, 9, 10-19.
void LaserDataInf::Unknown00() {}
int LaserDataInf::Initialize(void *) { return 0; }
int LaserDataInf::Update() { return 0; }
int LaserDataInf::Draw() { return 0; }
int LaserDataInf::Unknown10()
{
    previous->next = next;
    if (next) next->previous = previous;
    return 0;
}
int LaserDataInf::ClearInRadius(const CameraVector *, float, int, int) { return 0; }
int LaserDataInf::Unknown18(int, int, int, int) { return 0; }
int LaserDataInf::Unknown20(int, int, int, int) { return 0; }
int LaserDataInf::Clear(int, int) { return 0; }
int LaserDataInf::Unknown24(int, int) { return 0; }
void LaserDataInf::Unknown28() {}
void LaserDataInf::Unknown2c() {}
void LaserDataInf::Unknown30() {}
void LaserDataInf::Unknown34() {}
void LaserDataInf::Unknown38() {}
void LaserDataInf::Unknown3c() {}
void LaserDataInf::Unknown40() {}
void LaserDataInf::Unknown44() {}
void LaserDataInf::Unknown48() {}
void LaserDataInf::Unknown4c() {}

// Both derived lasers replace slot 4 with an empty result (0x426220, 0x427720).
int LaserLineInf::Unknown10() { return 0; }
int LaserInfiniteInf::Unknown10() { return 0; }
