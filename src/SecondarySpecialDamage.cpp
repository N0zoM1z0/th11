#include "SecondaryProjectile.hpp"

namespace
{
__forceinline AnimationVmView *SpecialDamageVm(unsigned int id)
{
    if (!id) return 0;
    BoundVmListsView *lists = reinterpret_cast<BoundVmListsView *>(g_RenderBatch);
    for (BoundVmNodeView *node = lists->first; node; node = node->next)
        if (node->vm->id == id) return reinterpret_cast<AnimationVmView *>(node->vm);
    for (BoundVmNodeView *node = lists->second; node; node = node->next)
        if (node->vm->id == id) return reinterpret_cast<AnimationVmView *>(node->vm);
    return 0;
}
}

int __stdcall SampleSecondarySpecialDamage3(SecondaryDamageEnemyView *self,
    const CameraVector *position, const float *extent)
{
    float distance = (position->x - self->x) * (position->x - self->x) +
        (position->y - self->y) * (position->y - self->y);
    if (distance < 2304.0f) return 50;
    float minX = static_cast<float>(position->x - extent[0] * 0.5);
    float minY = static_cast<float>(position->y - extent[1] * 0.5);
    float maxX = static_cast<float>(position->x + extent[0] * 0.5);
    float maxY = static_cast<float>(position->y + extent[1] * 0.5);
    float left = static_cast<float>(self->x - 16.0);
    float top = static_cast<float>((self->y - 224.0) - 224.0);
    float right = static_cast<float>(self->x + 16.0);
    float bottom = static_cast<float>((self->y - 224.0) + 224.0);
    if (maxX < left || maxY < top || minX > right || minY > bottom) return 0;
    return 8;
}

int __stdcall SampleSecondarySpecialDamage4(SecondaryDamageEnemyView *self, const CameraVector *position)
{
    int damage = 0;
    for (int i = 0; i < 5; ++i)
    {
        AnimationVmView *vm = SpecialDamageVm(self->animationId);
        if (!vm) self->animationId = 0;
        BoundVmNodeView *node = reinterpret_cast<BoundVmNodeView *>(
            reinterpret_cast<unsigned int>(vm) + 0x10);
        while (node)
        {
            AnimationVmView *candidate = reinterpret_cast<AnimationVmView *>(node->vm);
            SecondaryProjectileAnimationView *animation = reinterpret_cast<SecondaryProjectileAnimationView *>(candidate);
            if (animation->script == g_SecondarySpecialDamageScripts[i])
            {
                if (candidate)
                {
                    CameraVector first = { animation->position3dc.x + animation->position3e8.x,
                        animation->position3dc.y + animation->position3e8.y,
                        animation->position3dc.z + animation->position3e8.z };
                    CameraVector second = { animation->position3f4.x + first.x,
                        animation->position3f4.y + first.y, animation->position3f4.z + first.z };
                    CameraVector world = second;
                    world.x = static_cast<float>(world.x - 224.0);
                    world.y = static_cast<float>(world.y - 16.0);
                    float distance = (position->x - world.x) * (position->x - world.x) +
                        (position->y - world.y) * (position->y - world.y);
                    if (distance < animation->scale40 * animation->scale40) damage += 20;
                }
                break;
            }
            node = node->next;
        }
    }
    return damage;
}
