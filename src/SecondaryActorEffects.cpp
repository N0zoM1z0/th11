#include "SecondaryActorState.hpp"
#include "SecondaryPlayerController.hpp"
#include "SecondaryEffectPolar.hpp"
#include "Random.hpp"
#include "AngleMath.hpp"
#include <string.h>

extern RandomStateView g_TextRandomState;

namespace
{
// The input RNG is a full unsigned 32-bit sample. In the native x87 code it
// is first rounded to single precision, then scaled with 64-bit constants.
__forceinline float EffectRandomAngle()
{
    float sample = static_cast<float>(RandomNext32(&g_TextRandomState));
    float centered = static_cast<float>(sample * 4.656612873077392578125e-10 - 1.0);
    return static_cast<float>(centered * 3.1415927410125732421875);
}
__forceinline float EffectRandomScale()
{
    float sample = static_cast<float>(RandomNext32(&g_TextRandomState));
    float unit = static_cast<float>(sample * 2.3283064365386962890625e-10);
    return static_cast<float>(0.5 + unit * 0.5);
}
}

// Complete native 004101D0..00410398: independent effect-emission owner.
// The actor's twelve physical counter slots occupy bytes +4..+0x33; the
// original walks indices 0..10, then zeroes all 48 bytes, not only eleven.
void __stdcall EmitSecondaryActorEffects(SecondaryActorEffectView *effect,
    const CameraVector *origin)
{
    float angle = EffectRandomAngle();
    int *counts = reinterpret_cast<int *>(effect) + 1;
    for (int category = 0; category < 11; ++category)
    {
        for (int count = 0; count < counts[category]; ++count)
        {
            SecondaryEffectPolarVector ellipse;
            ellipse.SetPolar(angle, effect->width, effect->height);
            float scale = EffectRandomScale();
            CameraVector position;
            position.x = static_cast<float>(origin->x + scale * ellipse.x);
            position.y = static_cast<float>(origin->y + scale * ellipse.y);
            position.z = static_cast<float>(origin->z + 0.0);
            SpawnSecondaryPlayerDrop(g_SecondaryControllerDrops, &position,
                category + 1, 0xffffffffu, -1.5707963705062866f, 2.200000047683716f);
            float variation = EffectRandomAngle();
            float adjusted = static_cast<float>(angle + 1.57079637050628662109375 +
                variation * 0.25);
            angle = NormalizeAngle(adjusted);
        }
    }
    memset(counts, 0, 0x30);
}
