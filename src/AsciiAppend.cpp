#include "AsciiAppend.hpp"

#include <string.h>

void __stdcall AsciiManagerAppendString(AsciiManagerView *self,
                                      const float *position, const char *text)
{
    if (self->stringCount >= 320)
        return;

    AsciiStringRecord *record = &self->strings[self->stringCount++];
    strcpy(record->text, text);
    memcpy(record->position, position, sizeof(record->position));
    record->color = self->color;
    record->scale[0] = self->scale[0];
    record->scale[1] = self->scale[1];
    record->unknown11c = self->unknown1848c;
    record->unknown120 = self->unknown18498;
    record->unknown124 = self->unknown18494;
    record->unknown128 = self->unknown1849c;
    record->remainingFrames = self->stringLifetime;
}
