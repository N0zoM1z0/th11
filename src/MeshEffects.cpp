#include "MeshEffects.hpp"
#include "MainRuntime.hpp"
#include <new>

struct MeshEffectActorView
{
    unsigned char unknown000[0x10];
    AnimationFileView *file;
    unsigned char unknown014[0x87c - 0x14];
    CameraVector position;
    unsigned char unknown888[0x8bb0 - 0x888];
    StageTimerView timer8bb0;
};

struct MeshEffectSpellView
{
    unsigned char unknown000[0x88c];
    int age;
    unsigned char unknown890[0x8e0 - 0x890];
    unsigned int flags, value8e4;
};

struct MeshEffectEnemyView
{
    unsigned char unknown000[0x3c];
    int active;
};

struct MeshEffectEventView
{
    unsigned char unknown000[0x14];
    unsigned int count;
};

extern MeshEffectActorView *g_MeshEffectActor;
extern MeshEffectSpellView *g_MeshEffectSpell;
extern MeshEffectEnemyView *g_MeshEffectEnemy;
extern MeshEffectEventView *g_MeshEffectEvent;
extern int g_MeshEffectSelection0, g_MeshEffectSelection1;

struct MeshEffectSoundRecordView
{
    short value;
    unsigned char unknown002[6];
};

extern MeshEffectSoundRecordView g_MeshEffectSoundRecords[];
struct MeshEffectSoundQueueView
{
    int slots[12], pending[12], frames[12][128];
};
extern MeshEffectSoundQueueView g_MeshEffectSoundQueue;
extern int g_MeshEffectSoundValues[];

void __stdcall QueueMeshEffectSound(int sound)
{
    int value = g_MeshEffectSoundRecords[sound].value;
    int index;
    for (index = 0; index < 12; ++index)
    {
        if (g_MeshEffectSoundQueue.slots[index] < 0)
            break;
        if (g_MeshEffectSoundQueue.slots[index] == sound)
        {
            int count = g_MeshEffectSoundQueue.pending[index];
            if (count < 128)
            {
                g_MeshEffectSoundQueue.frames[index][count] = 0;
                ++g_MeshEffectSoundQueue.pending[index];
            }
            return;
        }
    }
    if (index < 12)
    {
        g_MeshEffectSoundQueue.slots[index] = sound;
        g_MeshEffectSoundValues[sound] = value;
        g_MeshEffectSoundQueue.frames[index][0] = 0;
        ++g_MeshEffectSoundQueue.pending[index];
    }
}

template<int Scale>
static __forceinline void AddMeshEffectJitter(float &coordinate)
{
    float value = static_cast<float>(static_cast<float>(RandomNext32(&g_TextRandomState)) *
        (2.0 / 4294967296.0) - 1.0);
    value = static_cast<float>(value * static_cast<double>(Scale));
    coordinate += value;
}

template<int Scale>
static __forceinline void AddMeshEffectSample(float &coordinate, unsigned int sample)
{
    float value = static_cast<float>(static_cast<float>(sample) * (2.0 / 4294967296.0) - 1.0);
    value = static_cast<float>(value * static_cast<double>(Scale));
    coordinate += value;
}

static __forceinline void SetMeshEffectTimer(StageTimerView &timer, int value)
{
    if (!(timer.flags & 1))
    {
        timer.fraction = 0.0f;
        timer.current = 0;
        timer.previous = -999999;
        timer.scale = &g_AnimationTimeScale;
        timer.flags |= 1;
    }
    timer.current = value;
    timer.fraction = static_cast<float>(value);
    timer.previous = value - 1;
}

void __cdecl UpdateMeshEffectSpellState()
{
    MeshEffectSpellView *spell = g_MeshEffectSpell;
    unsigned int flags = spell->flags;
    if (flags & 1)
    {
        if (spell->age >= 60)
        {
            spell->value8e4 = 0;
            spell->flags = flags & ~0x22u;
        }
        else if (g_MeshEffectEnemy->active &&
            g_MeshEffectSelection1 + g_MeshEffectSelection0 * 3 != 5)
            spell->flags = flags | 0x20;
    }
}

AnimationVmId __stdcall CreatePositionedAnimationVmId(AnimationFileView *file,
    const CameraVector *position, int script, int layer)
{
    if (g_MainRuntime.flags & 0x8000)
    {
        EnterCriticalSection(&g_MainRuntime.criticalSections[9]);
        ++g_MainRuntime.criticalDepth[9];
    }
    AnimationVmView *vm = AllocateSceneAnimationVm();
    vm->flags.value |= 0x40000000;
    vm->persistent020 = layer;
    CameraVector &vmPosition = *reinterpret_cast<CameraVector *>(&vm->persistent3e8);
    vmPosition.x = static_cast<float>(position->x + 32.0 + 192.0);
    vmPosition.y = static_cast<float>(position->y + 16.0);
    vmPosition.z = position->z;
    BindAnimationScript(file, script, vm);
    AnimationVmId id = PublishSceneAnimationVm(vm);
    if (g_MainRuntime.flags & 0x8000)
    {
        LeaveCriticalSection(&g_MainRuntime.criticalSections[9]);
        --g_MainRuntime.criticalDepth[9];
    }
    return id;
}

int __stdcall InitializeColumnMeshEffect(MeshEffectControllerView *self)
{
    self->parameter490 = 32.0f;
    self->parameter494 = 10.0f;
    MeshEffectActorView *actor = g_MeshEffectActor;
    self->origin = actor->position;
    self->origin.y = static_cast<float>(self->origin.y - 32.0);
    self->vmId = CreatePositionedAnimationVmId(actor->file, &self->origin, 0x2b, 0x16);
    CameraVector origin = self->origin;
    origin.x = static_cast<float>(origin.x - 60.0);
    origin.y = static_cast<float>(origin.y + 64.0);
    StageScriptMesh *oldMesh = self->mesh;
    if (oldMesh)
    {
        ReleaseVmBindingBlock(reinterpret_cast<VmBindingBlockView *>(oldMesh));
        operator delete(oldMesh);
    }
    self->mesh = 0;
    self->mesh = new StageScriptMesh(4, (static_cast<int>(origin.y) + 32) / 32 + 2, 1);
    ResetStageMesh(self->mesh, origin.x, 0.0f, 120.0f,
        static_cast<float>(origin.y + 32.0));

    RenderVertex *vertex = self->mesh->vertices;
    CameraVector *position = self->mesh->positions;
    for (int column = 0; column < self->mesh->columns; ++column, ++position, ++vertex)
    {
        unsigned int sample = RandomNext32(&g_TextRandomState);
        AddMeshEffectSample<4>(position->x, sample);
        sample = RandomNext32(&g_TextRandomState);
        AddMeshEffectSample<4>(position[self->mesh->columns].x, sample);
        sample = RandomNext32(&g_TextRandomState);
        AddMeshEffectSample<4>(position[self->mesh->columns * 2].x, sample);
        sample = RandomNext32(&g_TextRandomState);
        AddMeshEffectSample<4>(position[self->mesh->columns * 3].x, sample);
        vertex[self->mesh->columns].color = column < self->mesh->columns - 1 ? 0xffff8080 : 0xffffffff;
        vertex[self->mesh->columns * 2].color = column < self->mesh->columns - 1 ? 0xffff8080 : 0xffffffff;
    }
    MeshEffectSpellView *spell = g_MeshEffectSpell;
    unsigned int flags = spell->flags;
    if (flags & 1)
    {
        if (spell->age >= 60)
        {
            spell->value8e4 = 0;
            spell->flags = flags & ~0x22u;
        }
        else if (g_MeshEffectEnemy->active &&
            g_MeshEffectSelection1 + g_MeshEffectSelection0 * 3 != 5)
            spell->flags = flags | 0x20;
    }
    SetMeshEffectTimer(g_MeshEffectActor->timer8bb0, 120);
    ++g_MeshEffectEvent->count;
    return 0;
}

int __stdcall InitializeSquareMeshEffect(MeshEffectControllerView *self)
{
    self->parameter490 = 32.0f;
    self->parameter494 = 10.0f;
    MeshEffectActorView *actor = g_MeshEffectActor;
    self->origin = actor->position;
    QueueMeshEffectSound(0x31);
    self->vmId = CreatePositionedAnimationVmId(actor->file, &self->origin, 0x25, 0x16);
    AnimationVmView *vm = FindAnimationVmById(g_RenderBatch, self->vmId);
    if (!vm)
        self->vmId = 0;
    reinterpret_cast<MeshEffectVmView *>(vm)->callback = SquareMeshVmCallback;
    StageScriptMesh *oldMesh = self->mesh;
    if (oldMesh)
    {
        ReleaseVmBindingBlock(reinterpret_cast<VmBindingBlockView *>(oldMesh));
        operator delete(oldMesh);
    }
    self->mesh = 0;
    self->mesh = new StageScriptMesh(13, 13, 0);
    ResetStageMesh(self->mesh, static_cast<float>(self->origin.x - 17.0),
        static_cast<float>(self->origin.y - 17.0), 34.0f, 34.0f);
    UpdateMeshEffectSpellState();
    ++g_MeshEffectEvent->count;
    return 0;
}

int __fastcall SquareMeshVmCallback(AnimationVmView *vm)
{
    MeshEffectVmView *self = reinterpret_cast<MeshEffectVmView *>(vm);
    int segments = self->pointCount - 1;
    float increment = static_cast<float>(self->phase) / segments;
    RenderVertex *vertex = self->vertices;
    for (int i = 0; i < segments; ++i)
    {
        float *y = &vertex->y;
        AddMeshEffectJitter<8>(vertex->x);
        AddMeshEffectJitter<8>(*y);
        ++vertex;
        y = &vertex->y;
        AddMeshEffectJitter<8>(vertex->x);
        AddMeshEffectJitter<8>(*y);
        ++vertex;
    }
    float endV = vertex[-1].v + increment;
    *vertex = self->vertices[0];
    vertex->v = endV;
    vertex[1] = self->vertices[1];
    vertex[1].v = endV;
    return 0;
}
