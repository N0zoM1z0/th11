#pragma once
#include "SecondaryPlayerController.hpp"
#include "SecondaryActorRuntime.hpp"
#include "StageResource.hpp"
#include <stddef.h>

struct SecondaryDropPolarVector
{
    float x, y;
    void SetPolar(float angle, float speed);
};

// Complete physical record stride independently attested by the native pool
// walk. Do not assign an original item class identity to this layout.
struct SecondaryDropRecord
{
    AnimationVmView animation;            // +0x000, 0x434-byte VM span
    CameraVector position;                // +0x434
    CameraVector velocity;                // +0x440
    float extra44c;                       // +0x44c
    StageTimerView timer;                  // +0x450
    int state, kind, visibleKind, unknown470, group;
};

// Ordinary slots 0..149 and special historical slots 150..2197 are the
// physically observed 0x478-byte stride array. The independent counters
// start after it at native manager +0x265E68/+0x265E6C.
struct SecondaryControllerDropManager
{
    unsigned char unknown000[0x14];
    SecondaryDropRecord records[150 + 2048];
    unsigned int unknown265e64;
    int historyHead, historyCount;
};

typedef char SecondaryDropRecordSize[(sizeof(SecondaryDropRecord) == 0x478) ? 1 : -1];
typedef char SecondaryDropStateOffset[(offsetof(SecondaryDropRecord, state) == 0x464) ? 1 : -1];
typedef char SecondaryDropPositionOffset[(offsetof(SecondaryDropRecord, position) == 0x434) ? 1 : -1];
typedef char SecondaryDropTimerOffset[(offsetof(SecondaryDropRecord, timer) == 0x450) ? 1 : -1];
typedef char SecondaryDropPoolOffset[(offsetof(SecondaryControllerDropManager, records) == 0x14) ? 1 : -1];
typedef char SecondaryDropHistoryOffset[(offsetof(SecondaryControllerDropManager, records) + 150 * sizeof(SecondaryDropRecord) == 0x29e64) ? 1 : -1];
typedef char SecondaryDropHistoryHead[(offsetof(SecondaryControllerDropManager, historyHead) == 0x265e68) ? 1 : -1];
typedef char SecondaryDropHistoryCount[(offsetof(SecondaryControllerDropManager, historyCount) == 0x265e6c) ? 1 : -1];
