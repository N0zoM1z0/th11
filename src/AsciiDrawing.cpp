#include "AsciiDrawing.hpp"
#include "MainRuntime.hpp"

namespace
{
struct AsciiDrawingVmView
{
    unsigned char unknown000[0x3c];
    float scaleX, scaleY;
    unsigned char unknown044[0x4c - 0x44];
    float width, height;
    unsigned char unknown054[0x374 - 0x54];
    union
    {
        unsigned int value;
        struct { unsigned char blue, green, red, alpha; } bytes;
    } color;
    unsigned char unknown378[0x3ac - 0x378];
    AnimationSpriteView *sprite;
    unsigned char unknown3b0[0x3dc - 0x3b0];
    CameraVector position;
    unsigned char unknown3e8[0x404 - 0x3e8];
    unsigned int flags;
};

struct AsciiCameraRuntimeView
{
    unsigned char unknown000[0x204];
    PerspectiveCameraView alternateCamera;
    unsigned char unknown2f4[0x31c - 0x2f4];
    PerspectiveCameraView normalCamera;
    unsigned char unknown40c[0x54c - 0x40c];
    PerspectiveCameraView *currentCamera;
    unsigned int cameraMode;
};

typedef char AsciiDrawColorOffset[(offsetof(AsciiDrawingVmView, color) == 0x374) ? 1 : -1];
typedef char AsciiDrawSpriteOffset[(offsetof(AsciiDrawingVmView, sprite) == 0x3ac) ? 1 : -1];
typedef char AsciiDrawPositionOffset[(offsetof(AsciiDrawingVmView, position) == 0x3dc) ? 1 : -1];
typedef char AsciiDrawFlagsOffset[(offsetof(AsciiDrawingVmView, flags) == 0x404) ? 1 : -1];
typedef char AsciiCameraCurrentOffset[(offsetof(AsciiCameraRuntimeView, currentCamera) == 0x54c) ? 1 : -1];

__forceinline void ApplyNormalAsciiCamera()
{
    AsciiCameraRuntimeView *runtime = reinterpret_cast<AsciiCameraRuntimeView *>(&g_MainRuntime);
    runtime->currentCamera = &runtime->normalCamera;
    ApplyPerspectiveCamera(&runtime->normalCamera);
    static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice)->SetViewport(&runtime->currentCamera->viewport);
    runtime->cameraMode = 1;
}

__forceinline void ApplyAlternateAsciiCamera()
{
    AsciiCameraRuntimeView *runtime = reinterpret_cast<AsciiCameraRuntimeView *>(&g_MainRuntime);
    runtime->currentCamera = &runtime->alternateCamera;
    ApplyPerspectiveCamera(&runtime->alternateCamera);
    static_cast<IDirect3DDevice9 *>(g_MainRuntime.d3dDevice)->SetViewport(&runtime->currentCamera->viewport);
    runtime->cameraMode = 0;
}

__forceinline void DrawAsciiGlyph(AnimationVmView *vm)
{
    BuildAnimationVmQuad(vm, reinterpret_cast<CameraVector *>(&g_AnimationQuad[0].x),
        reinterpret_cast<CameraVector *>(&g_AnimationQuad[1].x),
        reinterpret_cast<CameraVector *>(&g_AnimationQuad[2].x),
        reinterpret_cast<CameraVector *>(&g_AnimationQuad[3].x));
    DrawAnimationVm(g_RenderBatch, vm, 1);
}
}

int __stdcall DrawAsciiManager(AsciiManagerObject *self, int mode)
{
    AnimationVmView *vm = reinterpret_cast<AnimationVmView *>(&self->textVm);
    AsciiDrawingVmView *text = reinterpret_cast<AsciiDrawingVmView *>(vm);
    text->flags = (text->flags & ~0x280000u) | 0x140001;
    int cameraMode = 1;
    for (int row = 0; row < self->stringCount; ++row)
    {
        AsciiStringRecord *record = &self->strings[row];
        float advance = 7.0f;
        if (record->unknown128 != static_cast<unsigned int>(mode))
            continue;
        text->position = *reinterpret_cast<CameraVector *>(record->position);
        text->flags |= 8;
        text->scaleX = record->scale[0];
        text->scaleY = record->scale[1];
        float lineHeight;
        if (record->unknown120 == 1)
        {
            text->flags &= ~0x80000000u;
            advance *= record->scale[0];
            lineHeight = 9.0f;
        }
        else if (record->unknown120 == 2)
        {
            text->flags &= ~0x80000000u;
            advance *= record->scale[0];
            lineHeight = 10.0f;
        }
        else if (record->unknown120 == 3)
        {
            text->flags &= ~0x80000000u;
            advance = static_cast<float>(record->scale[0] * 12.0);
            lineHeight = 16.0f;
        }
        else
        {
            if (record->scale[0] == 1.0f)
                text->flags &= ~0x80000000u;
            else
                text->flags |= 0x80000000u;
            advance = static_cast<float>(static_cast<int>(self->drawMode)) * record->scale[0];
            lineHeight = 14.0f;
        }
        if (cameraMode != static_cast<int>(record->unknown11c))
        {
            cameraMode = record->unknown11c;
            FlushRenderBatch(g_RenderBatch);
            if (cameraMode)
                ApplyAlternateAsciiCamera();
            else
                ApplyNormalAsciiCamera();
        }
        for (const unsigned char *character = reinterpret_cast<const unsigned char *>(record->text);
             *character; ++character)
        {
            unsigned int glyph = *character;
            if (glyph == '\n')
            {
                text->position.y += record->scale[1] * lineHeight;
                text->position.x = record->position[0];
                continue;
            }
            if (glyph == ' ')
            {
                text->position.x += advance;
                continue;
            }
            switch (record->unknown120)
            {
            case 0:
                SetAnimationSpriteUv(vm, &self->animation->sprites[glyph - 0x20]);
                break;
            case 1:
                SetAnimationSpriteUv(vm, &self->animation->sprites[glyph + 0x42]);
                break;
            case 2:
                advance = static_cast<float>(record->scale[0] * 7.0);
                if (glyph == '/') glyph = 237;
                else if (glyph == '+') glyph = 238;
                else if (glyph == '-') glyph = 239;
                else if (glyph == '*') glyph = 240;
                else if (glyph == '%') glyph = 241;
                else if (glyph == '$') glyph = 260;
                else if (glyph == '.')
                {
                    SetAnimationSpriteUv(vm, &self->animation->sprites[242]);
                    advance = static_cast<float>(record->scale[0] * 4.0);
                    break;
                }
                else glyph += 0xb3;
                SetAnimationSpriteUv(vm, &self->animation->sprites[glyph]);
                break;
            case 3:
                if (glyph == '/') glyph = 253;
                else if (glyph == '.') glyph = 254;
                else if (glyph == 's') glyph = 255;
                else glyph += 0xc3;
                SetAnimationSpriteUv(vm, &self->animation->sprites[glyph]);
                break;
            }
            text->flags |= 8;
            text->width = text->sprite->width;
            text->height = text->sprite->height;
            if (record->unknown124)
            {
                text->color.value = record->color & 0xff000000;
                text->position.x = static_cast<float>(text->position.x + 2.0);
                text->color.bytes.alpha = static_cast<unsigned char>(record->color >> 25);
                text->position.y = static_cast<float>(text->position.y + 2.0);
                DrawAsciiGlyph(vm);
                text->position.x = static_cast<float>(text->position.x - 2.0);
                text->position.y = static_cast<float>(text->position.y - 2.0);
            }
            text->color.value = record->color;
            DrawAsciiGlyph(vm);
            text->position.x += advance;
        }
    }
    if (cameraMode)
    {
        FlushRenderBatch(g_RenderBatch);
        ApplyNormalAsciiCamera();
    }
    return 1;
}

int __stdcall DrawAsciiOverlay(AsciiManagerObject *self)
{
    FlushRenderBatch(g_RenderBatch);
    DrawAsciiManager(self, 1);
    FlushRenderBatch(g_RenderBatch);
    ApplyNormalAsciiCamera();
    return 1;
}
