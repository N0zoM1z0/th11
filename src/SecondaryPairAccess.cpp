#include "SecondaryPairAccess.hpp"

void __stdcall SetSecondaryPairMode(SecondaryPairTween *self, int mode)
{
    self->mode = mode;
}

void __stdcall SetSecondaryPairInitial(SecondaryPairTween *self, const SecondaryVector2 *value)
{
    self->initial = *value;
}

void __stdcall SetSecondaryPairTarget(SecondaryPairTween *self, const SecondaryVector2 *value)
{
    self->target = *value;
}

void __stdcall ResetSecondaryPairControl1(SecondaryPairTween *self)
{
    const unsigned int *preset = reinterpret_cast<const unsigned int *>(&g_SecondaryPairControlPreset);
    unsigned int *control = reinterpret_cast<unsigned int *>(&self->control1);
    control[0] = preset[0];
    control[1] = preset[1];
}

void __stdcall ResetSecondaryPairControl2(SecondaryPairTween *self)
{
    const unsigned int *preset = reinterpret_cast<const unsigned int *>(&g_SecondaryPairControlPreset);
    unsigned int *control = reinterpret_cast<unsigned int *>(&self->control2);
    control[0] = preset[0];
    control[1] = preset[1];
}

// Native private register ABI: EAX is the pair state and ECX is duration.
// Keep the original decorated symbol while expressing the four-byte owner
// directly; this leaf is not a normal stack-argument C++ call target.
__declspec(naked) void __stdcall SetSecondaryPairDuration(SecondaryPairTween *, int)
{
    __asm {
        mov dword ptr [eax + 34h], ecx
        ret
    }
}

void __stdcall ResetSecondaryPairTimer(SecondaryPairTween *self)
{
    if (!(self->time.flags & 1))
    {
        self->time.fraction = 0.0f;
        self->time.current = 0;
        self->time.previous = -999999;
        self->time.scale = &g_AnimationTimeScale;
        self->time.flags |= 1;
    }
    self->time.current = 0;
    self->time.previous = -1;
    self->time.fraction = 0.0f;
}

int __stdcall GetSecondaryPairDuration(const SecondaryPairTween *self)
{
    return self->duration;
}

void __stdcall StopSecondaryPairTween(SecondaryPairTween *self)
{
    self->duration = 0;
}

SecondaryVector2 *__stdcall SubtractSecondaryPair(SecondaryVector2 *output,
    const SecondaryVector2 *left, const SecondaryVector2 *right)
{
    output->x = left->x - right->x;
    output->y = left->y - right->y;
    return output;
}

SecondaryVector2 *__stdcall ScaleSecondaryPair(SecondaryVector2 *output,
    const SecondaryVector2 *input, float scale)
{
    output->x = input->x * scale;
    output->y = input->y * scale;
    return output;
}

// Native private register ABI: EAX=output, ECX=left, EDX=right.
// The x87 evaluation order is observable in the original 16-byte leaf.
__declspec(naked) SecondaryVector2 *__stdcall AddSecondaryPair(
    SecondaryVector2 *, const SecondaryVector2 *, const SecondaryVector2 *)
{
    __asm {
        fld dword ptr [ecx]
        fadd dword ptr [edx]
        fstp dword ptr [eax]
        fld dword ptr [ecx + 4]
        fadd dword ptr [edx + 4]
        fstp dword ptr [eax + 4]
        ret
    }
}
