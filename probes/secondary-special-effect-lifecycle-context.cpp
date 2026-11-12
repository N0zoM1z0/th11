#include "../src/MeshEffects.hpp"
struct MeshEffectActorView;
extern int __stdcall InitializeSecondaryEffect(MeshEffectActorView *self);
extern int __stdcall UpdateSecondaryEffect(MeshEffectControllerView *self);
extern int __stdcall DrawSecondaryEffect(MeshEffectControllerView *self);
extern "C" int __stdcall SecondarySpecialEffectLifecycleProbe(MeshEffectControllerView *self,
    int mode)
{
    if (mode == 0) return InitializeSecondaryEffect(reinterpret_cast<MeshEffectActorView *>(self));
    if (mode == 1) return UpdateSecondaryEffect(self);
    return DrawSecondaryEffect(self);
}
