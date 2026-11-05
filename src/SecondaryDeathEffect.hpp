#pragma once
#include "SecondaryPlayerDeath.hpp"

struct SecondaryDeathEffect;
struct SecondaryDeathEffectNode
{
    SecondaryDeathEffect *effect;
    SecondaryDeathEffectNode *next, *previous;
    __forceinline void Reset(SecondaryDeathEffect *owner)
    {
        effect = owner;
        next = 0;
        previous = 0;
    }
    __forceinline void Append(SecondaryDeathEffectNode *node)
    {
        SecondaryDeathEffectNode *tail = this;
        while (tail->next) tail = tail->next;
        if (tail->next)
        {
            node->next = tail->next;
            tail->next->previous = node;
        }
        tail->next = node;
        node->previous = tail;
    }
};
struct SecondaryDeathEffect
{
    CameraVector position;
    SecondaryDeathEffectNode node;
    unsigned int unknown018;
    char text[64];
    unsigned int unknown05c;
    int lifetime, unknown064, mode, duration, delay;
    unsigned int unknown074, unknown078;
    float scale;
    unsigned int unknown080, color;
    SecondaryDeathEffect();
};
struct SecondaryDeathEffectManager
{
    unsigned char unknown000[0x78];
    __forceinline SecondaryDeathEffectNode *Head(unsigned int index)
    {
        return reinterpret_cast<SecondaryDeathEffectNode *>(
            reinterpret_cast<unsigned char *>(this) + 0x78 + index * sizeof(SecondaryDeathEffectNode));
    }
};
extern SecondaryDeathEffectManager *g_SecondaryDeathEffects;
extern unsigned int g_StageVariant;
extern int g_SecondaryDeathEffectMode, g_SecondaryDeathEffectLifetime0, g_SecondaryDeathEffectLifetime1;
extern const char g_SecondaryDeathEffectText[];
SecondaryDeathEffect *__stdcall AllocateSecondaryDeathEffect(SecondaryDeathEffectManager *manager,
    const CameraVector *position);

typedef char SecondaryDeathEffectSize[(sizeof(SecondaryDeathEffect) == 0x88) ? 1 : -1];
typedef char SecondaryDeathEffectLifetimeOffset[(offsetof(SecondaryDeathEffect, lifetime) == 0x60) ? 1 : -1];
typedef char SecondaryDeathEffectColorOffset[(offsetof(SecondaryDeathEffect, color) == 0x84) ? 1 : -1];
