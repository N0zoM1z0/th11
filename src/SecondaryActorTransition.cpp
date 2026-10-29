#include "SecondaryActorRuntime.hpp"

const char *__fastcall SelectSecondaryActorTransition(SecondaryActorManagerView *self)
{
    SecondaryActorStateView &state = self->state;
    int life = state.counter.value;
    state.counter.unknown008 = life;
    state.counter.baseline = 0;
    for (unsigned int i = 0; i < 8; ++i)
    {
        SecondaryActorSlot &slot = state.slots[i];
        if (slot.lifeThreshold < 0) continue;
        state.counter.unknown008 = static_cast<int>(static_cast<unsigned int>(life) - slot.lifeThreshold);
        state.counter.baseline = slot.lifeThreshold;
        if (life <= slot.lifeThreshold)
        {
            state.counter.value = slot.lifeThreshold;
            slot.lifeThreshold = -1;
            state.frame.SetFrame(-1);
            state.flags &= ~0x100000u;
            return slot.lifeScript;
        }
        break;
    }
    for (unsigned int i = 0; i < 8; ++i)
    {
        SecondaryActorSlot &slot = state.slots[i];
        if (slot.lifeThreshold < 0 || slot.frameLimit <= 0) continue;
        int remaining = static_cast<int>(static_cast<unsigned int>(slot.frameLimit) - state.frame.current);
        int seconds = remaining / 60;
        int hundredths = (remaining % 60) * 100 / 60;
        g_SecondaryActorHud->seconds = seconds > 99 ? 99 : seconds;
        g_SecondaryActorHud->hundredths = seconds > 99 ? 99 : hundredths;
        if (state.frame.current < slot.frameLimit) return 0;
        state.counter.value = slot.lifeThreshold;
        slot.lifeThreshold = -1;
        state.frame.SetFrame(-1);
        state.flags |= 0x100000;
        SecondaryActorSpellView *spell = SecondaryActorSpell();
        unsigned int flags = spell->flags;
        if (!(flags & 8))
        {
            if (flags & 1)
            {
                if (spell->age >= 60)
                {
                    spell->value8e4 = 0;
                    spell->flags = flags & ~0x22u;
                }
                else if (SecondaryActorEnemy()->active &&
                    static_cast<unsigned int>(g_MeshEffectSelection1) +
                        static_cast<unsigned int>(g_MeshEffectSelection0) * 3 != 5)
                    spell->flags = flags | 0x20;
            }
            g_SecondaryMaster->value018 = 0;
        }
        return slot.frameScript;
    }
    return 0;
}
