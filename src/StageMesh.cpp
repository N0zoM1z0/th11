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
    AnimationFileView *file;
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


}

AnimationVmView *__stdcall FindAnimationVmById(RenderBatchView *renderer, AnimationVmId id)
{
    if (!id.value)
        return 0;
    BoundVmListsView *lists = reinterpret_cast<BoundVmListsView *>(renderer);
    for (BoundVmNodeView *node = lists->first; node; node = node->next)
        if (node->vm->id == id.value)
            return reinterpret_cast<AnimationVmView *>(node->vm);
    for (BoundVmNodeView *node = lists->second; node; node = node->next)
        if (node->vm->id == id.value)
            return reinterpret_cast<AnimationVmView *>(node->vm);
    return 0;
}

AnimationVmId __stdcall CreateStageMeshRow(int columns)
{
    AnimationVmId id = CreateAnimationVmId(MeshRuntime()->file, 0x51, 0x1b);
    AnimationVmView *vm = FindAnimationVmById(g_RenderBatch, id);
    if (!vm)
        id.value = 0;
    MeshVm(vm)->vertices = static_cast<RenderVertex *>(malloc(static_cast<size_t>(columns) * 2 * sizeof(RenderVertex)));
    if (columns > 2)
    {
        MeshVm(vm)->flags = (MeshVm(vm)->flags & ~0x00c00000u) | 0x03000000u;
        MeshVm(vm)->pointCount = columns;
        RenderVertex *vertex = MeshVm(vm)->vertices;
        int count = static_cast<int>(static_cast<unsigned int>(columns) * 2);
        if (count > 0)
        {
            do
            {
                vertex->z = 0.0f;
                vertex->rhw = 1.0f;
                vertex->color = 0xffffffff;
                ++vertex;
            } while (--count);
        }
    }
    else
        MeshVm(vm)->flags &= ~0x03c00000u;
    return id;
}

AnimationVmId __stdcall CreateAlternateStageMeshRow(int columns)
{
    AnimationVmId id = CreateAnimationVmId(MeshRuntime()->file, 0x52, 0x1c);
    AnimationVmView *vm = FindAnimationVmById(g_RenderBatch, id);
    if (!vm)
        id.value = 0;
    RenderVertex *vertices = static_cast<RenderVertex *>(malloc(static_cast<size_t>(columns) * 2 * sizeof(RenderVertex)));
    unsigned int flags = (MeshVm(vm)->flags & ~0x00c00000u) | 0x03000000u;
    MeshVm(vm)->vertices = vertices;
    MeshVm(vm)->flags = flags;
    MeshVm(vm)->pointCount = columns;
    if (columns > 2)
    {
        int count = static_cast<int>(static_cast<unsigned int>(columns) * 2);
        if (count > 0)
        {
            do
            {
                vertices->z = 0.0f;
                vertices->rhw = 1.0f;
                vertices->color = 0xffffffff;
                ++vertices;
            } while (--count);
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
        rows = 0;
        columns = 0;
        ids = 0;
        rowVms = 0;
        vertices = 0;
        positions = 0;
        return;
    }
    rows = rowCount;
    columns = columnCount;
    ids = static_cast<AnimationVmId *>(malloc(static_cast<size_t>(rowCount) * 4 - 1));
    rowVms = static_cast<AnimationVmView **>(malloc(static_cast<size_t>(rowCount) * 4 - 1));
    vertices = static_cast<RenderVertex *>(malloc(static_cast<size_t>(rowCount) * columnCount * sizeof(RenderVertex)));
    positions = static_cast<CameraVector *>(malloc(static_cast<size_t>(rowCount) * columnCount * sizeof(CameraVector)));
    if (!flags)
        for (int row = 0; row < MeshRowLimit(rows); ++row)
        {
            ids[row] = CreateStageMeshRow(columnCount);
            AnimationVmView *vm = FindAnimationVmById(g_RenderBatch, ids[row]);
            if (!vm)
                ids[row] = 0;
            rowVms[row] = vm;
            MeshVm(rowVms[row])->callback = StageMeshVmCallback;
            MeshVm(rowVms[row])->mesh = this;
            MeshVm(rowVms[row])->flags &= ~0x70u;
        }
    else
        for (int row = 0; row < MeshRowLimit(rows); ++row)
        {
            ids[row] = CreateAlternateStageMeshRow(columnCount);
            AnimationVmView *vm = FindAnimationVmById(g_RenderBatch, ids[row]);
            if (!vm)
                ids[row] = 0;
            rowVms[row] = vm;
            MeshVm(rowVms[row])->callback = StageMeshVmCallback;
            MeshVm(rowVms[row])->mesh = this;
            MeshVm(rowVms[row])->flags &= ~0x70u;
        }
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
    CameraVector position;
    position.x = static_cast<float>(x + 192.0 + 32.0);
    float originY = static_cast<float>(y + 16.0);
    position.y = originY;
    position.z = 0.0f;
    float stepX = width / MeshRowLimit(self->rows);
    float stepY = height / MeshRowLimit(self->columns);
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
            position.y += stepY;
        }
        position.y = originY;
        position.x += stepX;
    }
    PublishStageMesh(self);
}
