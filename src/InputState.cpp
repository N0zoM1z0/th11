#include "InputState.hpp"

// TH11 0x00459B50..0x00459BB8. The caller owns current/previous publication.
void __fastcall UpdateInputRepeatState(InputStateView *self)
{
    unsigned int keys = self->current;
    unsigned int bit = 1;
    self->repeat = 0;
    self->held = 0;
    unsigned int *counter = self->heldFrames;
    int keysLeft = 32;
    do
    {
        if ((keys & 1) != 0)
        {
            ++*counter;
            if (*counter >= 8)
                self->held |= bit;
            if (*counter >= 26)
            {
                self->repeat |= bit;
                *counter -= 8;
            }
        }
        else
        {
            *counter = 0;
        }
        ++counter;
        keys >>= 1;
        bit <<= 1;
    } while (--keysLeft != 0);
    self->pressed = (self->previous ^ self->current) & self->current;
    self->released = (self->previous ^ self->current) & ~self->current;
}
