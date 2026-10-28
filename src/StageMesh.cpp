#include "StageMesh.hpp"
#include "MainRuntime.hpp"
#include <stdlib.h>

namespace
{
struct MeshRuntimeView
{
    unsigned char unknown000[0x1ac];
    int enabled;
    unsigned char unknown1b0[0x588 - 0x1b0];
    unsigned int file;
};

static __forceinline MeshRuntimeView *MeshRuntime()
{
    return reinterpret_cast<MeshRuntimeView *>(&g_MainRuntime);
}

static __forceinline StageMeshVmView *MeshVm(AnimationVmView *vm)
{
    return reinterpret_cast<StageMeshVmView *>(vm);
}

static __forceinline int MeshRowLimit(int rows)
{
    return static_cast<int>(static_cast<unsigned int>(rows) - 1);
}

static __forceinline void BindMeshRow(StageMeshView *self, int row, unsigned int id)
{
    self->ids[row] = id;
    AnimationVmView *vm = FindAnimationVmById(g_RenderBatch, self->ids[row]);
    if (!vm)
        self->ids[row] = 0;
    self->rowVms[row] = vm;
    MeshVm(self->rowVms[row])->callback = StageMeshVmCallback;
    MeshVm(self->rowVms[row])->mesh = self;
    MeshVm(self->rowVms[row])->flags &= ~0x70u;
}
}

AnimationVmView *__stdcall FindAnimationVmById(RenderBatchView *renderer, unsigned int id)
{
    if (!id)
        return 0;
    BoundVmListsView *lists = reinterpret_cast<BoundVmListsView *>(renderer);
    for (BoundVmNodeView *node = lists->first; node; node = node->next)
        if (node->vm->id == id)
            return reinterpret_cast<AnimationVmView *>(node->vm);
    for (BoundVmNodeView *node = lists->second; node; node = node->next)
        if (node->vm->id == id)
            return reinterpret_cast<AnimationVmView *>(node->vm);
    return 0;
}

unsigned int *__stdcall CreateStageMeshRow(unsigned int *id, int columns)
{
    CreateAnimationVmId(id, MeshRuntime()->file, 0x51, 0x1b);
    AnimationVmView *vm = FindAnimationVmById(g_RenderBatch, *id);
    if (!vm)
        *id = 0;
    MeshVm(vm)->vertices = static_cast<RenderVertex *>(malloc(static_cast<size_t>(columns) * 2 * sizeof(RenderVertex)));
    if (columns > 2)
    {
        MeshVm(vm)->flags = (MeshVm(vm)->flags & ~0x00c00000u) | 0x03000000u;
        MeshVm(vm)->pointCount = columns;
        RenderVertex *vertex = MeshVm(vm)->vertices;
        int count = static_cast<int>(static_cast<unsigned int>(columns) * 2);
        for (; count > 0; --count, ++vertex)
        {
            vertex->color = 0xffffffff;
            vertex->z = 0.0f;
            vertex->rhw = 1.0f;
        }
    }
    else
        MeshVm(vm)->flags &= ~0x03c00000u;
    return id;
}

unsigned int *__stdcall CreateAlternateStageMeshRow(unsigned int *id, int columns)
{
    CreateAnimationVmId(id, MeshRuntime()->file, 0x52, 0x1c);
    AnimationVmView *vm = FindAnimationVmById(g_RenderBatch, *id);
    if (!vm)
        *id = 0;
    RenderVertex *vertices = static_cast<RenderVertex *>(malloc(static_cast<size_t>(columns) * 2 * sizeof(RenderVertex)));
    unsigned int flags = (MeshVm(vm)->flags & ~0x00c00000u) | 0x03000000u;
    MeshVm(vm)->vertices = vertices;
    MeshVm(vm)->flags = flags;
    MeshVm(vm)->pointCount = columns;
    if (columns > 2)
    {
        int count = static_cast<int>(static_cast<unsigned int>(columns) * 2);
        for (; count > 0; --count, ++vertices)
        {
            vertices->color = 0xffffffff;
            vertices->z = 0.0f;
            vertices->rhw = 1.0f;
        }
    }
    else
        MeshVm(vm)->flags = flags & ~0x03c00000u;
    return id;
}

StageScriptMesh::StageScriptMesh(int rowCount, int columnCount, int flags)
{
    if (!MeshRuntime()->enabled)
    {
        rows = columns = 0;
        ids = 0;
        rowVms = 0;
        vertices = 0;
        positions = 0;
        return;
    }
    rows = rowCount;
    columns = columnCount;
    ids = static_cast<unsigned int *>(malloc(static_cast<size_t>(rowCount) * 4 - 1));
    rowVms = static_cast<AnimationVmView **>(malloc(static_cast<size_t>(rowCount) * 4 - 1));
    vertices = static_cast<RenderVertex *>(malloc(static_cast<size_t>(rowCount) * columnCount * sizeof(RenderVertex)));
    positions = static_cast<CameraVector *>(malloc(static_cast<size_t>(rowCount) * columnCount * sizeof(CameraVector)));
    unsigned int id;
    if (!flags)
        for (int row = 0; row < MeshRowLimit(rows); ++row)
            BindMeshRow(this, row, *CreateStageMeshRow(&id, columnCount));
    else
        for (int row = 0; row < MeshRowLimit(rows); ++row)
            BindMeshRow(this, row, *CreateAlternateStageMeshRow(&id, columnCount));
}

void __stdcall PublishStageMesh(StageMeshView *self)
{
    if (!self->rows)
        return;
    RenderVertex *source = self->vertices;
    for (int row = 0; row < MeshRowLimit(self->rows); ++row)
    {
        RenderVertex *destination = MeshVm(self->rowVms[row])->vertices;
        for (int column = 0; column < self->columns; ++column)
        {
            *destination++ = *source;
            *destination++ = source[self->columns];
            ++source;
        }
    }
}

int __fastcall StageMeshVmCallback(AnimationVmView *vm)
{
    PublishStageMesh(MeshVm(vm)->mesh);
    return 0;
}

void __stdcall ResetStageMesh(StageMeshView *self, float x, float y, float width, float height)
{
    if (!self->rows)
        return;
    CameraVector position = {static_cast<float>(x + 192.0 + 32.0), static_cast<float>(y + 16.0), 0.0f};
    float originY = position.y;
    width /= MeshRowLimit(self->rows);
    height /= MeshRowLimit(self->columns);
    CameraVector *source = self->positions;
    RenderVertex *vertex = self->vertices;
    for (int row = 0; row < self->rows; ++row)
    {
        for (int column = 0; column < self->columns; ++column, ++source, ++vertex)
        {
            *source = position;
            *reinterpret_cast<CameraVector *>(vertex) = position;
            vertex->u = static_cast<float>(source->x / 640.0);
            vertex->v = static_cast<float>(source->y / 480.0);
            if (vertex->u < 0.0f)
                vertex->u = 0.0f;
            if (vertex->v < 0.0f)
                vertex->v = 0.0f;
            vertex->color = 0xffffffff;
            vertex->rhw = 1.0f;
            position.y += height;
        }
        position.y = originY;
        position.x += width;
    }
    PublishStageMesh(self);
}
