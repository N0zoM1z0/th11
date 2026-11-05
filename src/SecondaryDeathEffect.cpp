#include "SecondaryDeathEffect.hpp"
#include <string.h>

inline SecondaryDeathEffect::SecondaryDeathEffect()
{
    memset(this, 0, sizeof(*this));
    node.Reset(this);
    scale = 1.0f;
    color = 0xffffffff;
    duration = 300;
    delay = -1;
}

SecondaryDeathEffect *__stdcall AllocateSecondaryDeathEffect(SecondaryDeathEffectManager *manager,
    const CameraVector *position)
{
    SecondaryDeathEffect *effect = new SecondaryDeathEffect;
    manager->Head(g_StageVariant)->Append(&effect->node);
    effect->delay = 30;
    effect->position = *position;
    strncpy(effect->text, g_SecondaryDeathEffectText, sizeof(effect->text));
    effect->unknown064 = 0;
    effect->mode = g_SecondaryDeathEffectMode;
    effect->lifetime = g_SecondaryDeathEffectLifetime1;
    if (!g_SecondaryDeathEffectMode) effect->lifetime = g_SecondaryDeathEffectLifetime0;
    return effect;
}

void __stdcall ApplySecondaryPlayerDeathPenalty(const CameraVector *position)
{
    SecondaryDeathEffectManager *manager = g_SecondaryDeathEffects;
    SecondaryDeathEffectNode *node = manager->Head(g_StageVariant)->next;
    while (node)
    {
        SecondaryDeathEffectNode *next = node->next;
        SecondaryDeathEffect *effect = node->effect;
        if (effect->delay > 0)
            effect->delay = static_cast<int>(static_cast<unsigned int>(effect->delay) - 1);
        node = next;
    }
    SecondaryDeathEffect *effect = AllocateSecondaryDeathEffect(manager, position);
    effect->lifetime = static_cast<int>(static_cast<unsigned int>(effect->lifetime) - 60);
    if (effect->lifetime <= 0) effect->lifetime = 1;
    effect->duration = 90;
    effect->color = 0xffff8080;
    effect->delay = 30;
}
