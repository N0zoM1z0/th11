#pragma once

#include <d3d9.h>
#include <stddef.h>

struct AnimationVmView;
struct AnimationFileView;
struct AnimationVmNode
{
    AnimationVmView *vm;
    AnimationVmNode *next;
    AnimationVmNode *previous;

    __forceinline void Reset(AnimationVmView *owner)
    {
        next = 0;
        previous = 0;
        vm = owner;
    }
};

extern float g_AnimationTimeScale;

struct AnimationTimerView
{
    int previous;
    int current;
    float fraction;
    float *scale;
    unsigned int flags;

    __forceinline void ResetFields()
    {
        current = 0;
        fraction = 0.0f;
        previous = -999999;
    }

    __forceinline void SetFrame(int value)
    {
        if (!(flags & 1))
        {
            fraction = 0.0f;
            current = 0;
            previous = -999999;
            scale = &g_AnimationTimeScale;
            flags |= 1;
        }
        fraction = 0.0f;
        current = 0;
        previous = value;
    }
};

struct AnimationPersistentState
{
    unsigned int words[3];
};

union AnimationVmFlags
{
    unsigned int value;
    struct { unsigned short low, high; } halves;
};

// Observed 0x434-byte reset/bind span; unknown storage is retained.
struct AnimationVmView
{
    unsigned int unknown000;
    AnimationVmNode updateNode;
    AnimationVmNode drawNode;
    unsigned int unknown01c;
    unsigned int persistent020;
    unsigned char unknown024[0x3c - 0x24];
    float scaleX, scaleY;
    unsigned char unknown044[0x5c - 0x44];
    AnimationTimerView time;
    unsigned char unknown070[0xd4 - 0x70];
    int reset0d4;
    unsigned char unknown0d8[0x120 - 0xd8];
    int reset120;
    unsigned char unknown124[0x14c - 0x124];
    int reset14c;
    unsigned char unknown150[0x198 - 0x150];
    int reset198;
    unsigned char unknown19c[0x1d4 - 0x19c];
    int reset1d4;
    unsigned char unknown1d8[0x220 - 0x1d8];
    int reset220;
    unsigned char unknown224[0x24c - 0x224];
    int reset24c;
    unsigned char unknown250[0x278 - 0x250];
    int reset278;
    unsigned char unknown27c[0x2a4 - 0x27c];
    int reset2a4;
    unsigned char unknown2a8[0x2b4 - 0x2a8];
    D3DMATRIX transform;
    unsigned char unknown2f4[0x374 - 0x2f4];
    int spriteIndex;
    unsigned char unknown378[0x39e - 0x378];
    unsigned short fileId;
    unsigned short unknown3a0;
    unsigned short scriptId;
    unsigned char *scriptStart;
    unsigned char *instruction;
    unsigned int unknown3ac;
    AnimationFileView *file;
    unsigned char unknown3b4[0x3e8 - 0x3b4];
    AnimationPersistentState persistent3e8;
    unsigned char unknown3f4[0x404 - 0x3f4];
    AnimationVmFlags flags;
    unsigned char unknown408[0x434 - 0x408];
};

struct AnimationTextureEntry
{
    IDirect3DTexture9 *texture;
    unsigned char unknown04[16];
};

struct AnimationFileView
{
    unsigned short id;
    unsigned char unknown002[0x11c - 2];
    unsigned char **scripts;
    AnimationTextureEntry *textures;
    int bindingDisabled;
};

typedef char AnimationVmSize[(sizeof(AnimationVmView) == 0x434) ? 1 : -1];
typedef char AnimationVmTimeOffset[(offsetof(AnimationVmView, time) == 0x5c) ? 1 : -1];
typedef char AnimationVmTransformOffset[(offsetof(AnimationVmView, transform) == 0x2b4) ? 1 : -1];
typedef char AnimationVmFlagsOffset[(offsetof(AnimationVmView, flags) == 0x404) ? 1 : -1];
typedef char AnimationVmFileOffset[(offsetof(AnimationVmView, file) == 0x3b0) ? 1 : -1];
typedef char AnimationTextureEntrySize[(sizeof(AnimationTextureEntry) == 20) ? 1 : -1];
typedef char AnimationFileScriptOffset[(offsetof(AnimationFileView, scripts) == 0x11c) ? 1 : -1];
typedef char AnimationFileTextureOffset[(offsetof(AnimationFileView, textures) == 0x120) ? 1 : -1];

void __stdcall ResetAnimationVm(AnimationVmView *self);
void __stdcall BindAnimationScript(AnimationFileView *file, int scriptId, AnimationVmView *self);
int __stdcall ExecuteAnimationVm(AnimationVmView *self);
