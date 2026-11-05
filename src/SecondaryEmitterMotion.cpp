#include "SecondaryEmitterMotion.hpp"
#include "AngleMath.hpp"
#include <math.h>
#pragma intrinsic(atan2, fabs)

namespace
{
__forceinline SecondaryEmitterMotionOwnerView *Player()
{
    return reinterpret_cast<SecondaryEmitterMotionOwnerView *>(g_SecondaryActorReferences);
}
__forceinline int Subtract(int left, int right)
{
    return static_cast<int>(static_cast<unsigned int>(left) - right);
}
}

void __stdcall OrbitSecondaryEmitter(SecondaryEmitterObjectView *self)
{
    float radius;
    if (!Player()->focused) radius = 64.0f;
    else radius = 24.0f;
    SecondaryEmitterPolarVector position;
    position.SetPolar(self->angle, radius);
    int x = static_cast<int>(position.x * -128.0);
    self->pendingX = Subtract(Player()->x, x);
    int y = static_cast<int>(position.y * -128.0);
    self->pendingY = Subtract(Player()->y, y);
    self->angle = NormalizeAngle(static_cast<float>(self->angle + static_cast<double>(0.10471976f)));
    if (self->pendingX < -0x6600)
    {
        self->flags |= 1;
        self->refresh = 1;
        self->pendingX = static_cast<int>(static_cast<unsigned int>(self->pendingX) + 0xcc00);
        return;
    }
    if (self->pendingX > 0x6600)
    {
        self->refresh = 1;
        self->pendingX = Subtract(self->pendingX, 0xcc00);
        self->flags |= 1;
        return;
    }
    if (self->flags & 1) self->refresh = 1;
    self->flags &= ~1u;
}

void __stdcall AimSecondaryEmitter(SecondaryEmitterObjectView *self)
{
    SecondaryEmitterMotionOwnerView *player = Player();
    if (self->index == 0 && !player->focused &&
        (fabs(player->movement.x) > 0.001f || fabs(player->movement.y) > 0.001f))
    {
        float target = atan2(player->rememberedMovement.y, player->rememberedMovement.x);
        float magnitude = static_cast<float>(fabs(static_cast<double>(
            NormalizeAngle(SecondaryDamageAngleDifference(target, player->angle)))));
        if (magnitude < static_cast<double>(0.19634955f))
            player->angle = target;
        else
        {
            float difference = NormalizeAngle(SecondaryDamageAngleDifference(target, player->angle));
            float absolute = fabs(difference);
            if (absolute < static_cast<double>(0.7853982f))
                player->angle = NormalizeAngle(static_cast<float>(
                    player->angle + NormalizeAngle(static_cast<float>(difference / 3.0))));
            else
                player->angle = NormalizeAngle(static_cast<float>(
                    player->angle + NormalizeAngle(static_cast<float>(difference / 5.0))));
        }
    }
    float angle = NormalizeAngle(self->polar[player->focused].x + player->angle);
    self->angle = angle;
    SecondaryEmitterPolarVector position;
    position.SetPolar(angle, self->polar[player->focused].y);
    int x = static_cast<int>(position.x * -128.0);
    self->pendingX = Subtract(Player()->x, x);
    int y = static_cast<int>(position.y * -128.0);
    self->pendingY = Subtract(Player()->y, y);
}

float __stdcall SecondaryDamageAngleDifference(float target, float current)
{
    if (target - current > static_cast<double>(3.1415927f))
        return static_cast<float>(target - (current + static_cast<double>(6.2831855f)));
    if (current - target > static_cast<double>(3.1415927f))
        return static_cast<float>(target - (current - static_cast<double>(6.2831855f)));
    return target - current;
}
