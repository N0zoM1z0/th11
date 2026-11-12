#include "SecondaryPlayerController.hpp"
#include "SecondaryGameplay.hpp"
#include "SecondaryDamage.hpp"

// Native 0x00431E00..0x00431F6D: full 366-byte player rectangle check.
// The first bounds use caller dimensions scaled by double 1/2; a miss
// falls through to a separate 24.0-pixel proximity rectangle. This is
// not a general-purpose bounding-box intersection or a clamped test.
int __stdcall CheckSecondaryPlayerRectangle(const float *extent,
    const CameraVector *position)
{
    SecondaryPlayerControllerView *owner =
        reinterpret_cast<SecondaryPlayerControllerView *>(g_SecondaryActorReferences);
    float left = static_cast<float>(position->x - extent[0] * 0.5);
    float top = static_cast<float>(position->y - extent[1] * 0.5);
    float right = static_cast<float>(position->x + extent[0] * 0.5);
    float bottom = static_cast<float>(position->y + extent[1] * 0.5);
    if (!(right < owner->minimum.x || bottom < owner->minimum.y ||
        owner->maximum.x < left || owner->maximum.y < top))
    {
        SecondaryGameplayHudView *hud =
            reinterpret_cast<SecondaryGameplayHudView *>(g_SecondaryActorHud);
        if ((!hud || !hud->dialogue) && owner->mode != 2 &&
            owner->mode != 4 && owner->mode != 3 && !(owner->flags & 2))
        {
            if (owner->statusTime.current <= 0)
                BeginSecondaryCollision(reinterpret_cast<SecondaryCollisionReferencesView *>(owner));
            return 1;
        }
    }
    else
    {
        left = static_cast<float>(position->x - 24.0);
        top = static_cast<float>(position->y - 24.0);
        right = static_cast<float>(position->x + 24.0);
        bottom = static_cast<float>(position->y + 24.0);
        if (!(right < owner->minimum.x || bottom < owner->minimum.y ||
            owner->maximum.x < left || owner->maximum.y < top)) return 2;
    }
    return 0;
}
