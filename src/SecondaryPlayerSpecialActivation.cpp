#include "SecondaryPlayerController.hpp"
#include "SecondaryActorRuntime.hpp"
#include "MeshEffects.hpp"
#include "StageResource.hpp"

// The complete original switch owns 241 code bytes, three alignment bytes,
// and the full six-entry native dispatch table through 0x40661B.
// These original independent mode callees still have source-open owners.
struct MeshEffectActorView;
extern MeshEffectActorView *g_MeshEffectActor;
void __stdcall InitializeSecondaryEffect(MeshEffectActorView *self);
void __stdcall InitializeSecondarySpecialMode1(MeshEffectActorView *self);
void __stdcall InitializeSecondarySpecialMode4(MeshEffectActorView *self);
void __stdcall InitializeSecondarySpecialMode5(MeshEffectActorView *self);

namespace
{
struct SpecialActivationView
{
    unsigned char unknown000[0x14];
    StageTimerView timer014;
    unsigned char unknown028[0x3c - 0x28];
    int active;
    unsigned char unknown040[0x498 - 0x40];
    int spellReady;
};
}

// 0x406510..0x40661B: complete native secondary special startup.
// The native does not check the globally published owner or spell for null.
int __stdcall ActivateSecondaryPlayerSpecial()
{
    SpecialActivationView *owner = reinterpret_cast<SpecialActivationView *>(g_MeshEffectActor);
    if (owner->active != 0) return -1;
    owner->active = 1;
    owner->timer014.SetFrame(-1);
    SecondaryActorSpellView *spell = SecondaryActorSpell();
    owner->spellReady = ((spell->flags & 1) && spell->age >= 60) ? 1 : 0;
    QueueSecondaryActorSound(0x26, g_SecondaryActorReferences->position.x);
    const int mode = static_cast<int>(static_cast<unsigned int>(g_MeshEffectSelection0) * 3u + g_MeshEffectSelection1);
    switch (mode)
    {
    case 0: InitializeSecondaryEffect(g_MeshEffectActor); break;
    case 1: InitializeSecondarySpecialMode1(g_MeshEffectActor); break;
    case 2: InitializeSquareMeshEffect(reinterpret_cast<MeshEffectControllerView *>(g_MeshEffectActor)); break;
    case 3: InitializeColumnMeshEffect(reinterpret_cast<MeshEffectControllerView *>(g_MeshEffectActor)); break;
    case 4: InitializeSecondarySpecialMode4(g_MeshEffectActor); break;
    case 5: InitializeSecondarySpecialMode5(g_MeshEffectActor); break;
    }
    return 0;
}
