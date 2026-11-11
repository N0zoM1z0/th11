#include "SecondaryPlayerMovement.hpp"
#include "ChainRegistration.hpp"
#include "AnimationDocument.hpp"
#include "ErrorLog.hpp"
#include <stddef.h>

// TH11 v1.00a 0042F8A0..0042FEB4: load the active player animation and
// pattern, register its two chain callbacks, and prepare movement bounds.
// Each unknown global has an observed original-PE address, but its original
// translation unit and data symbol name have not been established.
extern SecondaryPatternConfigurationView *g_SecondaryPlayerPreparedConfiguration;
extern const char *g_SecondaryPlayerPatternFilenames[];
extern int g_SecondaryPlayerPatternCapacity, g_SecondaryPlayerPatternCapacityUnit;
extern const float g_SecondaryPlayerHalfSizeX[];
extern const float g_SecondaryPlayerHalfSizeY[];
extern const float g_SecondaryPlayerHalfSizeZ[];
extern const float g_SecondaryPlayerHalfDepth[];
extern const char g_SecondaryPlayerDataMissingMessage[];
extern const char g_SecondaryPlayerAnimationPath0[], g_SecondaryPlayerAnimationPath1[];

namespace
{
struct SecondaryPlayerConfigurationPrefix
{
    unsigned char unknown000[0x10];
    float normal, focus, diagonal, focusDiagonal;
    int powerLevels, capacityUnit;
};
typedef char SecondaryConfigurationVelocityOffset[
    (offsetof(SecondaryPlayerConfigurationPrefix, normal) == 0x10) ? 1 : -1];
typedef char SecondaryConfigurationPowerOffset[
    (offsetof(SecondaryPlayerConfigurationPrefix, powerLevels) == 0x20) ? 1 : -1];

static __forceinline SecondaryPlayerConfigurationPrefix *Config(
    SecondaryPlayerControllerView *owner)
{
    return reinterpret_cast<SecondaryPlayerConfigurationPrefix *>(owner->configuration);
}

static __forceinline ChainElement *CreatePlayerChainElement(
    SecondaryPlayerControllerView *owner, ChainCallback callback)
{
    ChainElement *element = new ChainElement;
    if (element)
    {
        element->flags &= ~1u;
        element->callback = 0;
        element->unknown0c = 0;
        element->auxiliaryCallback = 0;
        element->priority = 0;
        element->link.element = element;
        element->link.next = 0;
        element->link.previous = 0;
    }
    element->callback = callback;
    element->unknown0c = 0;
    element->auxiliaryCallback = 0;
    element->context = owner;
    element->flags = (element->flags & ~2u) | 1u;
    return element;
}

static __forceinline void ResetInitialPlayerTime(StageTimerView *timer, int frame)
{
    if (!(timer->flags & 1))
    {
        timer->fraction = 0.0f;
        timer->current = 0;
        timer->previous = -999999;
        timer->scale = &g_AnimationTimeScale;
        timer->flags |= 1;
    }
    timer->previous = frame - 1;
    timer->current = frame;
    timer->fraction = static_cast<float>(frame);
}

static __forceinline CameraVector Minus(const CameraVector &a, const CameraVector &b)
{
    CameraVector result = {a.x - b.x, a.y - b.y, a.z - b.z};
    return result;
}
static __forceinline CameraVector Plus(const CameraVector &a, const CameraVector &b)
{
    CameraVector result = {a.x + b.x, a.y + b.y, a.z + b.z};
    return result;
}
}

// Ordinary source declaration. Native caller 004301B0 pushes one owner,
// and optimizer-private registers for its ten dependencies are not faked.
int __stdcall InitializeSecondaryPlayerController(SecondaryDamageReferencesView *self)
{
    SecondaryPlayerControllerView *owner =
        reinterpret_cast<SecondaryPlayerControllerView *>(self);
    SecondaryPlayerMovementView *movement =
        reinterpret_cast<SecondaryPlayerMovementView *>(owner);

    const char *animationName = g_MeshEffectSelection0 ?
        g_SecondaryPlayerAnimationPath1 : g_SecondaryPlayerAnimationPath0;
    AnimationDocumentView *document = AcquireAnimationDocument(7, animationName);
    *reinterpret_cast<AnimationDocumentView **>(
        reinterpret_cast<unsigned char *>(owner) + 0x10) = document;
    if (!document)
    {
        AppendLog(&g_RuntimeLog.buffer, g_SecondaryPlayerDataMissingMessage);
        return -1;
    }

    if (g_SecondaryPlayerPreparedConfiguration)
    {
        owner->configuration = g_SecondaryPlayerPreparedConfiguration;
        g_SecondaryPlayerPreparedConfiguration = 0;
    }
    else
    {
        unsigned int shot = static_cast<unsigned int>(g_MeshEffectSelection0) * 3u +
            static_cast<unsigned int>(g_MeshEffectSelection1);
        if (LoadSecondaryProjectilePatterns(self, g_SecondaryPlayerPatternFilenames[shot]))
        {
            AppendLog(&g_RuntimeLog.buffer, g_SecondaryPlayerDataMissingMessage);
            return -1;
        }
    }

    ChainElement *update = CreatePlayerChainElement(
        owner, reinterpret_cast<ChainCallback>(UpdateSecondaryPlayerCallback));
    AddToUpdateChain(update, 16);
    *reinterpret_cast<ChainElement **>(
        reinterpret_cast<unsigned char *>(owner) + 8) = update;

    ChainElement *draw = CreatePlayerChainElement(
        owner, reinterpret_cast<ChainCallback>(DrawSecondaryPlayerCallback));
    AddToDrawChain(draw, 22);
    *reinterpret_cast<ChainElement **>(
        reinterpret_cast<unsigned char *>(owner) + 12) = draw;

    ResetAnimationVm(&owner->animation);
    owner->animation.unknown408[0x14] = 16;
    owner->animation.unknown408[0x15] = 16;
    BindAnimationScript(reinterpret_cast<AnimationFileView *>(document),
                        0, &owner->animation);

    owner->position.x = 0.0f;
    owner->position.y = 400.0f;
    owner->x = 0;
    owner->y = 0xc800;

    SecondaryPlayerConfigurationPrefix *preset = Config(owner);
    const double speedScale = 128.0;
    movement->normalSpeed = static_cast<int>(preset->normal * speedScale);
    movement->focusedSpeed = static_cast<int>(preset->focus * speedScale);
    movement->normalDiagonalSpeed = static_cast<int>(preset->diagonal * speedScale);
    movement->focusedDiagonalSpeed = static_cast<int>(
        preset->focusDiagonal * speedScale);
    g_SecondaryPlayerPatternCapacity = preset->powerLevels * preset->capacityUnit;
    g_SecondaryPlayerPatternCapacityUnit = preset->capacityUnit;

    // The original timer begins at -1, with previous -2, then advances.
    ResetInitialPlayerTime(&owner->patternTime, -1);

    unsigned int character = static_cast<unsigned int>(g_MeshEffectSelection0);
    preset->normal = g_SecondaryPlayerHalfSizeX[character];
    preset->diagonal = g_SecondaryPlayerHalfSizeZ[character];
    preset->focus = g_SecondaryPlayerHalfSizeY[character];

    owner->halfSize.x = static_cast<float>(preset->normal * 0.5);
    owner->halfSize.y = owner->halfSize.x;
    owner->halfSize.z = 5.0f;
    owner->size.x = static_cast<float>(g_SecondaryPlayerHalfSizeY[character] * 0.5);
    owner->size.y = owner->size.x;
    owner->size.z = 5.0f;
    owner->otherSize.x = static_cast<float>(g_SecondaryPlayerHalfDepth[character] * 0.5);
    owner->otherSize.y = owner->otherSize.x;
    owner->otherSize.z = 5.0f;

    owner->minimum = Minus(owner->position, owner->halfSize);
    owner->maximum = Plus(owner->position, owner->halfSize);
    owner->scaledMinimum = Minus(owner->position, owner->size);
    owner->scaledMaximum = Plus(owner->position, owner->size);
    owner->otherMinimum = Minus(owner->position, owner->otherSize);
    owner->otherMaximum = Plus(owner->position, owner->otherSize);
    owner->fullMinimum = Minus(owner->position, owner->otherSize);
    owner->fullMaximum = Plus(owner->position, owner->otherSize);

    ResetInitialPlayerTime(&owner->stateTime, 0);
    owner->stateTime.previous = -1;
    owner->stateTime.current = 0;
    owner->stateTime.fraction = 0.0f;

    ResetInitialPlayerTime(&owner->statusTime, 120);
    movement->patternSlot = 0;
    movement->blend = 30;
    return 0;
}
