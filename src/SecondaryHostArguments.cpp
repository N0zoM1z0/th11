#include "SecondaryActorInstructionControl.hpp"

int __stdcall ReadSecondaryHostInteger(SecondaryActorStateView *self, int index)
{
    return ReadSecondaryIntArgument(self->owner->current, index);
}

float __stdcall ReadSecondaryHostReal(SecondaryActorStateView *self, int index)
{
    return ReadSecondaryFloatArgument(self->owner->current, index);
}

int *__stdcall SecondaryHostIntegerDestination(SecondaryActorStateView *self)
{
    SecondaryVm *vm = self->owner->current;
    if (vm->instruction->references & 1)
    {
        int reference = reinterpret_cast<SecondaryValue *>(vm->instruction + 1)->integer;
        if (reference >= 0)
            return reinterpret_cast<int *>(vm->stack.data + vm->stack.frame + reference);
        return vm->owner->GlobalIntPointer(reference);
    }
    return 0;
}

float *__stdcall SecondaryHostFloatDestination(SecondaryActorStateView *self)
{
    return SecondaryFloatDestination(self->owner->current, 0);
}

int __stdcall ReadSecondaryHostIntegerValue(SecondaryActorStateView *self, int index, int value)
{
    SecondaryVm *vm = self->owner->current;
    if (vm->instruction->references & (1u << index))
        value = ReadSecondaryIntReference(vm, value);
    return value;
}

float __stdcall ReadSecondaryHostRealValue(SecondaryActorStateView *self, int index, float value)
{
    SecondaryVm *vm = self->owner->current;
    if (vm->instruction->references & (1u << index))
        value = ReadSecondaryFloatReference(vm, value);
    return value;
}
