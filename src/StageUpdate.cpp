#include "StageUpdate.hpp"
#include "AngleMath.hpp"
#include "MainRuntime.hpp"
#include <math.h>

namespace
{
const double kColumnWaveStep = static_cast<double>(0.6981317400932312f);
const double kRowWaveStep = static_cast<double>(1.2566370964050293f);
const double kPhaseXStep = static_cast<double>(0.04908738657832146f);
const double kPhaseYStep = static_cast<double>(0.039269909262657166f);
const double kQuarterTurn = static_cast<double>(1.5707963705062866f);
const double kPi = static_cast<double>(3.1415927410125732f);
}

static __forceinline StageMeshView *StageMesh(StageResourceView *self)
{
    return reinterpret_cast<StageMeshView *>(self->bindings);
}

static __forceinline float StageSine(float angle)
{
    return static_cast<float>(sin(static_cast<double>(angle)));
}

int __stdcall UpdateStageObjects(StageResourceView *self)
{
    for (int index = 0; index < self->data->objectCount; ++index)
    {
        StageObjectView *object = reinterpret_cast<StageObjectView *>(self->objects[index]);
        if (object->flags & 1)
        {
            StagePrimitiveView *primitive = reinterpret_cast<StagePrimitiveView *>(object + 1);
            int live = 0;
            if (primitive->kind >= 0)
            {
                do
                {
                    AnimationVmView *vm = reinterpret_cast<AnimationVmView *>(
                        static_cast<unsigned char *>(self->vms) + primitive->vmIndex * static_cast<int>(sizeof(AnimationVmView)));
                    ExecuteAnimationVm(vm);
                    if (vm->instruction)
                        ++live;
                    primitive = reinterpret_cast<StagePrimitiveView *>(
                        reinterpret_cast<unsigned char *>(primitive) + primitive->byteSize);
                } while (primitive->kind >= 0);
            }
            if (!live)
                object->flags &= ~1u;
        }
    }
    return 0;
}

int __stdcall UpdateStage(StageResourceView *self)
{
    if ((self->flags2ff0 & 8) ||
        ((self->flags2ff0 & 4) && self->timer2ff4.current >= 60))
        return 1;

    self->camera.view.offsetX = self->camera.view.offsetY = 0.0f;
    CameraVector displacement = {0.0f, 0.0f, 0.0f};
    self->camera.updateOffset = displacement;
    D3DXVec3Normalize(reinterpret_cast<CameraVector *>(self->camera.view.unknown024),
        &self->camera.view.targetOffset);
    self->tint = 0x00808080;
    if (!(self->flags2ff0 & 4) || self->timer2ff4.current < 30)
    {
        UpdateStageObjects(self);
        ExecuteStageScript(self);
        StageEmbeddedVm *vm = self->vms180;
        for (int i = 0; i < 8; ++i, ++vm)
            ExecuteAnimationVm(reinterpret_cast<AnimationVmView *>(vm));
        if (self->accent)
        {
            float scale = g_AnimationTimeScale;
            g_AnimationTimeScale = 1.0f;
            ExecuteAnimationVm(reinterpret_cast<AnimationVmView *>(&self->vms2348[0]));
            ExecuteAnimationVm(reinterpret_cast<AnimationVmView *>(&self->vms2348[1]));
            ExecuteAnimationVm(reinterpret_cast<AnimationVmView *>(&self->vms2348[2]));
            g_AnimationTimeScale = scale;
        }
    }
    self->accent = 0;
    *reinterpret_cast<StageCameraView *>(reinterpret_cast<unsigned char *>(&g_MainRuntime) + 0x434) = self->camera;
    if (self->bindings)
    {
        CameraVector *position = StageMesh(self)->positions;
        float phaseX = self->phaseX;
        float phaseY = self->phaseY;
        if (self->distortionMode == 1)
        {
            ResetStageMesh(StageMesh(self), -192.0f, 0.0f, 384.0f, 448.0f);
            RenderVertex *vertex = StageMesh(self)->vertices;
            for (int row = 0; row < StageMesh(self)->rows; ++row)
            {
                if (StageMesh(self)->columns > 0)
                {
                    float offsetY = static_cast<float>(StageSine(phaseY) * 12.0);
                    for (int column = 0; column < StageMesh(self)->columns; ++column, ++position, ++vertex)
                    {
                        reinterpret_cast<unsigned char *>(&vertex->color)[3] = 0xc0;
                        displacement.x = static_cast<float>(StageSine(phaseX) * 12.0);
                        displacement.y = offsetY;
                        if (row && column && row != StageMesh(self)->rows - 1 && column != StageMesh(self)->columns - 1)
                        {
                            vertex->x += displacement.x;
                            vertex->y += displacement.y;
                            vertex->z = 0.0f;
                            position->z = 0.0f;
                        }
                        phaseX = NormalizeAngle(static_cast<float>(phaseX + kColumnWaveStep));
                    }
                }
                phaseY = NormalizeAngle(static_cast<float>(phaseY - kRowWaveStep));
            }
            self->phaseX = NormalizeAngle(static_cast<float>(self->phaseX + kPhaseXStep));
            self->phaseY = NormalizeAngle(static_cast<float>(self->phaseY + kPhaseYStep));
        }
        else if (self->distortionMode == 2)
        {
            float radius = self->radius;
            if (self->minimumRadius < self->radius)
                self->radius = static_cast<float>(self->radius - 2.0);
            float diameter = static_cast<float>(radius * 2.0);
            ResetStageMesh(StageMesh(self), -radius, static_cast<float>(224.0 - radius), diameter, diameter);
            RenderVertex *vertex = StageMesh(self)->vertices;
            for (int row = 0; row < StageMesh(self)->rows; ++row)
            {
                double radiusSquared = radius * radius;
                for (int column = 0; column < StageMesh(self)->columns; ++column, ++position, ++vertex)
                {
                    displacement.x = static_cast<float>(position->x - 224.0);
                    displacement.y = static_cast<float>(position->y - 240.0);
                    displacement.z = position->z - displacement.z;
                    CameraVector direction = displacement;
                    float distance = displacement.x * displacement.x + displacement.y * displacement.y;
                    float remaining = static_cast<float>(radiusSquared - distance);
                    if (remaining >= 0.0f)
                    {
                        float factor = static_cast<float>(remaining / radiusSquared);
                        vertex->color = 0xffffffff;
                        unsigned char *color = reinterpret_cast<unsigned char *>(&vertex->color);
                        color[3] = 0x60;
                        color[2] = static_cast<unsigned char>(static_cast<int>(255.0 - (255 - color[2]) * factor));
                        color[1] = static_cast<unsigned char>(static_cast<int>(255.0 - (255 - color[1]) * factor));
                        color[0] = static_cast<unsigned char>(static_cast<int>(255.0 - (255 - color[0]) * factor));
                        float magnitude = static_cast<float>(factor * 32.0);
                        D3DXVec3Normalize(&direction, &direction);
                        direction.x *= magnitude;
                        direction.y *= magnitude;
                        direction.z *= magnitude;
                        direction.x = static_cast<float>(StageSine(phaseX) * factor * 8.0 + direction.x);
                        direction.y = static_cast<float>(StageSine(phaseY) * factor * 8.0 + direction.y);
                        vertex->x += direction.x;
                        vertex->y += direction.y;
                        vertex->z = 0.0f;
                        position->z = 0.0f;
                    }
                    else
                        reinterpret_cast<unsigned char *>(&vertex->color)[3] = 0;
                    phaseX = NormalizeAngle(static_cast<float>(phaseX + kQuarterTurn));
                    phaseY = NormalizeAngle(static_cast<float>(phaseY - kColumnWaveStep));
                }
            }
            self->phaseX = NormalizeAngle(static_cast<float>(self->phaseX + kPhaseXStep));
            float random = static_cast<float>(RandomNext32(&g_TextRandomState) * (1.0 / 4294967296.0));
            float step = static_cast<float>(random * kPi / 40.0 + kPhaseYStep);
            self->phaseY = NormalizeAngle(static_cast<float>(step + self->phaseY));
        }
    }
    self->reset300c = static_cast<int>(static_cast<unsigned int>(self->reset300c) + 1);
    return 1;
}
