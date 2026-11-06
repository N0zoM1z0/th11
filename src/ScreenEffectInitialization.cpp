#include "SecondaryHud.hpp"
#include "ChainRegistration.hpp"
#include <new>

// Address-taken chain callbacks; their bodies and original names remain open.
int __fastcall ScreenEffectUpdate0(void *context);
int __fastcall ScreenEffectUpdate1(void *context);
int __fastcall ScreenEffectUpdate2(void *context);
int __fastcall ScreenEffectUpdate4(void *context);
int __fastcall ScreenEffectUpdate6(void *context);
int __fastcall ScreenEffectUpdate8(void *context);
int __fastcall ScreenEffectDraw0(void *context);
int __fastcall ScreenEffectDraw2(void *context);
int __fastcall ScreenEffectDraw4(void *context);
int __fastcall ScreenEffectDraw6(void *context);
int __fastcall ScreenEffectDraw7(void *context);
int __fastcall ScreenEffectCleanup(void *context);

namespace
{
__forceinline void InitializeScreenEffectLink(ChainNode *link, ChainElement *element)
{
    link->element = element;
    link->next = 0;
    link->previous = 0;
}
struct ScreenEffectChainElement : ChainElement
{
    ScreenEffectChainElement()
    {
        callback = 0;
        unknown0c = 0;
        auxiliaryCallback = 0;
        priority = 0;
        flags &= ~1u;
        InitializeScreenEffectLink(&link, this);
    }
};
typedef char ScreenEffectChainElementSize[(sizeof(ScreenEffectChainElement) == 0x24) ? 1 : -1];
__forceinline ChainElement *CreateElement(ChainCallback callback)
{
    ChainElement *element = new ScreenEffectChainElement;
    element->callback = callback;
    return element;
}
__forceinline ChainElement *RegisterUpdate(SecondaryHudScreenEffect *self,
    ChainCallback callback)
{
    ChainElement *element = CreateElement(callback);
    element->unknown0c = 0;
    element->auxiliaryCallback = 0;
    element->context = self;
    element->flags |= 3;
    AddToUpdateChain(element, 14);
    return element;
}
__forceinline ChainElement *RegisterDraw(SecondaryHudScreenEffect *self,
    ChainCallback callback, int priority)
{
    ChainElement *element = CreateElement(callback);
    element->unknown0c = 0;
    element->auxiliaryCallback = 0;
    element->context = self;
    element->flags |= 3;
    AddToDrawChain(element, priority);
    return element;
}
}

void __stdcall InitializeSecondaryHudScreenEffect(SecondaryHudScreenEffect *self,
    int mode, int duration, int color0, int color1, int drawPriority)
{
    switch (mode)
    {
    case 0:
        self->update = RegisterUpdate(self, ScreenEffectUpdate0);
        self->draw = RegisterDraw(self, ScreenEffectDraw0, drawPriority);
        break;
    case 1:
        self->update = RegisterUpdate(self, ScreenEffectUpdate1);
        break;
    case 2:
        self->update = RegisterUpdate(self, ScreenEffectUpdate2);
        self->draw = RegisterDraw(self, ScreenEffectDraw2, drawPriority);
        break;
    case 3:
        self->value018 = 255;
        self->update = RegisterUpdate(self, ScreenEffectUpdate0);
        self->draw = RegisterDraw(self, ScreenEffectDraw2, drawPriority);
        break;
    case 5:
        self->update = RegisterUpdate(self, ScreenEffectUpdate2);
        self->draw = RegisterDraw(self, ScreenEffectDraw0, drawPriority);
        break;
    case 4:
        self->update = RegisterUpdate(self, ScreenEffectUpdate4);
        self->draw = RegisterDraw(self, ScreenEffectDraw4, drawPriority);
        break;
    case 6:
        self->update = RegisterUpdate(self, ScreenEffectUpdate6);
        self->draw = RegisterDraw(self, ScreenEffectDraw6, drawPriority);
        break;
    case 7:
        self->update = RegisterUpdate(self, ScreenEffectUpdate6);
        self->draw = RegisterDraw(self, ScreenEffectDraw7, drawPriority);
        break;
    case 8:
        self->update = RegisterUpdate(self, ScreenEffectUpdate8);
        break;
    default:
        break;
    }
    self->update->auxiliaryCallback = ScreenEffectCleanup;
    StageTimerView *time = &self->time;
    if (!(time->flags & 1))
    {
        time->fraction = 0.0f;
        time->current = 0;
        time->previous = -999999;
        time->scale = &g_AnimationTimeScale;
        time->flags |= 1;
    }
    time->fraction = 0.0f;
    time->current = 0;
    time->previous = -1;
    self->mode = mode;
    self->value028 = 0;
    self->duration = duration;
    self->color0 = color0;
    self->color1 = color1;
}

SecondaryHudScreenEffect *__stdcall CreateSecondaryHudScreenEffect(
    int mode, int duration, int color0, int color1, int drawPriority)
{
    SecondaryHudScreenEffect *self = new SecondaryHudScreenEffect;
    if (!self) return 0;
    InitializeSecondaryHudScreenEffect(self, mode, duration, color0, color1, drawPriority);
    return self;
}
