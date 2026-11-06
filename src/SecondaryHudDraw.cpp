#include "SecondaryHudDraw.hpp"
#include "SecondaryHud.hpp"
#include "AsciiFormatting.hpp"
#include "AnimationImmediateDraw.hpp"
#include "SecondaryActorRuntime.hpp"
#include <stdio.h>
#include <string.h>

namespace
{
struct HudTextPosition
{
    float values[3];
    HudTextPosition(float x, float y, float z)
    { values[0] = x; values[1] = y; values[2] = z; }
    operator const float *() const { return values; }
    float &operator[](int index) { return values[index]; }
};
typedef char HudTextPositionSize[(sizeof(HudTextPosition) == 12) ? 1 : -1];
__forceinline int &HudInteger(SecondaryHudView *self, unsigned int offset)
{ return *reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(self) + offset); }
__forceinline int &StateInteger(unsigned int offset)
{
    return *reinterpret_cast<int *>(
        reinterpret_cast<unsigned char *>(&g_SecondaryHostSelectionState) + offset);
}
__forceinline int SpellInteger(unsigned int offset)
{ return *reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(g_MeshEffectSpell) + offset); }
__forceinline AsciiManagerView *Ascii()
{ return reinterpret_cast<AsciiManagerView *>(g_AsciiManager); }
__forceinline unsigned char &Alpha(void *object, unsigned int offset)
{ return reinterpret_cast<unsigned char *>(object)[offset]; }
__forceinline unsigned int Color(AnimationVmView *vm)
{ return *reinterpret_cast<unsigned int *>(reinterpret_cast<unsigned char *>(vm) + 0x374); }
__forceinline AnimationVmView *RequiredVm(unsigned int &id)
{
    AnimationVmView *vm = FindAnimationVmById(g_RenderBatch, AnimationVmId(id));
    if (!vm) id = 0;
    return vm;
}
__forceinline int Multiply(int left, int right)
{ return static_cast<int>(static_cast<unsigned int>(left) * right); }
__forceinline int Add(int left, int right)
{ return static_cast<int>(static_cast<unsigned int>(left) + right); }
__forceinline void Scale(AsciiManagerView *self, float value)
{ self->scale[0] = value; self->scale[1] = value; }
}

int __stdcall DrawSecondaryHud(SecondaryHudView *self)
{
    AsciiManagerView *ascii;
    unsigned int &bonusId = reinterpret_cast<unsigned int &>(HudInteger(self, 0x43b4));
    if (!RequiredVm(bonusId))
        ascii = Ascii();
    else
    {
        char text[16];
        sprintf(text, "%d", self->clearBonus);
        float length = static_cast<float>(strlen(text));
        HudTextPosition position(static_cast<float>(224.0 - length * 0.5 * 12.0), 200.0f, 0.0f);
        AnimationVmView *bonus = RequiredVm(bonusId);
        ascii = Ascii();
        Alpha(ascii, 0x18483) = Alpha(bonus, 0x377);
        ascii->unknown1849c = 1;
        ascii->unknown18498 = 3;
        AsciiManagerPrint(ascii, position, text);
        ascii = Ascii();
        Alpha(ascii, 0x18483) = 0xff;
        ascii->unknown18498 = 0;
        ascii->unknown1849c = 0;
    }
    if (self->trackedActive)
    {
        HudTextPosition position(222.0f, 192.0f, 0.0f);
        AnimationVmView *tracked = RequiredVm(self->tracked);
        if (!g_MeshEffectSpell)
        {
            self->trackedActive = 0;
            ReleaseSecondaryAnimation(AnimationVmId(self->tracked));
            self->tracked = 0;
        }
        else if (!tracked)
            self->trackedActive = 0;
        else
        {
            Alpha(ascii, 0x18483) = Alpha(tracked, 0x377);
            ascii->unknown1849c = 1;
            ascii->unknown18498 = 3;
            int seconds = SpellInteger(0x8f8) / 60;
            AsciiManagerPrint(ascii, position, "%3d.", seconds < 1000 ? seconds : 999);
            ascii = Ascii();
            Scale(ascii, 0.6f);
            position[0] = 266.0f;
            position[1] = 198.0f;
            AsciiManagerPrint(ascii, position, "%.2ds", SpellInteger(0x8f8) % 60 * 100 / 60);
            ascii = Ascii();
            position[0] = 222.0f;
            position[1] = 208.0f;
            ascii->color = 0xff808080;
            Scale(ascii, 1.0f);
            Alpha(ascii, 0x18483) = Alpha(tracked, 0x377);
            int bestSeconds, bestHundredths;
            DecodeSecondaryHudBestTime(&bestSeconds, &bestHundredths);
            AsciiManagerPrint(ascii, position, "%3d.", bestSeconds);
            ascii = Ascii();
            Scale(ascii, 0.6f);
            position[0] = 266.0f;
            position[1] = 214.0f;
            AsciiManagerPrint(ascii, position, "%.2ds", bestHundredths);
            ascii = Ascii();
            Scale(ascii, 1.0f);
            Alpha(ascii, 0x18483) = 0xff;
            ascii->unknown18498 = 0;
            ascii->unknown1849c = 0;
            ascii->color = 0xffffffff;
        }
    }
    if (self->bossDisplayed > 0.0f)
    {
        float rectangle[4] = {41.0f, 23.0f, static_cast<float>(self->bossDisplayed * 330.0 + 41.0), 25.0f};
        DrawHudSolidRectangle(rectangle, 0xff000000);
        rectangle[0] = static_cast<float>(rectangle[0] - 1.0);
        rectangle[2] = static_cast<float>(rectangle[2] - 1.0);
        rectangle[1] = static_cast<float>(rectangle[1] - 1.0);
        rectangle[3] = static_cast<float>(rectangle[3] - 1.0);
        DrawHudSolidRectangle(rectangle, 0xffffffff);
        for (int i = 0; i < 4; ++i)
        {
            if (self->bars[i].fraction != 0.0f)
            {
                float fraction = self->bossDisplayed > self->bars[i].fraction ? self->bars[i].fraction : self->bossDisplayed;
                rectangle[0] = 40.0f;
                rectangle[2] = static_cast<float>(fraction * 330.0 + 40.0);
                rectangle[1] = 22.0f;
                rectangle[3] = 24.0f;
                DrawHudSolidRectangle(rectangle, static_cast<unsigned int>(self->bars[i].threshold));
            }
        }
    }
    for (int i = 0; i < 4; ++i) DrawAnimationVmByMode(g_RenderBatch, &self->gauges[i]);
    HudTextPosition position(508.0f, 48.0f, 0.0f);
    ascii = Ascii();
    ascii->unknown18498 = 3;
    Alpha(ascii, 0x18483) = Alpha(&self->primary[0], 0x377);
    int score, lastDigit;
    if (!(g_StageResourceFlags & 0x10))
    {
        score = StateInteger(0);
        lastDigit = StateInteger(0x60);
    }
    else
    {
        int record = *reinterpret_cast<int *>(g_SecondaryGameplayStatistics +
            Multiply(Add(g_MeshEffectSelection1, Multiply(g_MeshEffectSelection0, 3)), 0x68d4) +
            0x5a4 + Multiply(Add(g_StageVariant, Multiply(g_SecondaryActorDifficulty, 6)), 8));
        int current = HudInteger(self, 0x43e4);
        score = record < current ? current : record;
        lastDigit = 0;
    }
    if (score >= 100000000)
        AsciiManagerPrint(ascii, position, "%.9d%d", score, lastDigit);
    else
        AsciiManagerPrint(ascii, position, " %.8d%d", score, lastDigit);
    position = HudTextPosition(508.0f, 72.0f, 0.0f);
    score = HudInteger(self, 0x43e4);
    if (score >= 100000000)
        AsciiManagerPrint(Ascii(), position, "%.9d%d", score, StateInteger(0x5c));
    else
        AsciiManagerPrint(Ascii(), position, " %.8d%d", score, StateInteger(0x5c));
    position = HudTextPosition(520.0f, 128.0f, 0.0f);
    AsciiManagerPrint(Ascii(), position, "%d.", StateInteger(8) / StateInteger(0x6c));
    ascii = Ascii();
    Scale(ascii, 0.6f);
    position[0] = 540.0f;
    position[1] = static_cast<float>(position[1] + 7.0);
    AsciiManagerPrint(ascii, position, "%.2d", Multiply(StateInteger(8) % StateInteger(0x6c), 100) / StateInteger(0x6c));
    ascii = Ascii();
    Scale(ascii, 1.0f);
    position[1] = static_cast<float>(position[1] - 7.0);
    position[0] = 554.0f;
    AsciiManagerPrint(ascii, position, "/%d.", StateInteger(0x68) / StateInteger(0x6c));
    ascii = Ascii();
    position[0] = 586.0f;
    position[1] = static_cast<float>(position[1] + 7.0);
    Scale(ascii, 0.6f);
    AsciiManagerPrint(ascii, position, "00");
    ascii = Ascii();
    Scale(ascii, 1.0f);
    position = HudTextPosition(520.0f, 152.0f, 0.0f);
    AsciiManagerPrint(ascii, position, "%d", StateInteger(0x74));
    ascii = Ascii();
    position[0] = 48.0f;
    position[1] = 455.0f;
    position[2] = 0.0f;
    ascii->color = 0xffffffff;
    ascii->unknown18498 = 2;
    ascii->unknown1849c = 1;
    Alpha(ascii, 0x18483) = Alpha(&self->gauges[0], 0x377);
    int percent = StateInteger(0x14) >= 10000 ? 100 : Multiply(StateInteger(0x14), 100) / 10000;
    int bonus = StateInteger(0x74) / 100;
    if (bonus > 899) bonus = 899;
    bonus = Add(bonus, percent);
    int points = StateInteger(0x10) / 100;
    points = static_cast<int>(static_cast<unsigned int>(points) - points % 10);
    AsciiManagerPrint(ascii, position, "%.6d*%1.2f", points, bonus / 100.0);
    ascii = Ascii();
    ascii->color = 0xffffffff;
    Alpha(ascii, 0x18483) = 0xff;
    ascii->unknown18498 = 0;
    ascii->unknown1849c = 0;
    if (g_SecondaryMaster && self->seconds >= 0 &&
        *reinterpret_cast<int *>(g_SecondaryMaster->indexed01c) &&
        !(*reinterpret_cast<unsigned int *>(g_SecondaryMaster->indexed01c + 0x20) & 1) && !self->dialogue)
    {
        for (int i = 0; i < 2; ++i) DrawAnimationVmByMode(g_RenderBatch, &self->digits[i]);
        ascii = Ascii();
        position[0] = 394.0f;
        position[1] = 16.0f;
        position[2] = 0.0f;
        ascii->color = Color(&self->digits[0]);
        ascii->unknown18498 = 3;
        ascii->unknown1849c = 1;
        AsciiManagerPrint(ascii, position, ".");
        ascii = Ascii();
        position[0] = 402.0f;
        position[1] = 22.0f;
        Scale(ascii, 0.6f);
        AsciiManagerPrint(ascii, position, "%.2d", self->unknown4444);
        ascii = Ascii();
        Scale(ascii, 1.0f);
        ascii->color = 0xffffffff;
        ascii->unknown1849c = 0;
        ascii->unknown18498 = 0;
    }
    return 1;
}

int __fastcall SecondaryHudDrawCallback(SecondaryHudView *self)
{
    return DrawSecondaryHud(self);
}
