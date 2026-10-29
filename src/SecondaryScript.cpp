#include "SecondaryScript.hpp"
#include <stdlib.h>
#include <string.h>

// Native 45B5B0 is RET-only; callers still evaluate and pass their arguments.
void __cdecl TraceSecondaryScript(...) {}
// Reviewed math boundaries; these implementations remain outside this TU.
float __stdcall SecondaryUnaryMathA(float value);
float __stdcall SecondaryUnaryMathB(float value);
float __stdcall SecondaryAngleMath(float y, float x);
float __stdcall AnimationDistanceRoot(float value);
float __stdcall NormalizeAngle(float value);

namespace
{
__forceinline unsigned char *Payload(SecondaryInstruction *instruction)
{ return reinterpret_cast<unsigned char *>(instruction) + sizeof(*instruction); }
__forceinline SecondaryValue *Argument(SecondaryVm *self, int index)
{ return reinterpret_cast<SecondaryValue *>(Payload(self->instruction)) + index; }
__forceinline unsigned int ReferenceMask(int index) { return 1u << (index & 31); }
__forceinline int Add(int a, int b) { return static_cast<int>(static_cast<unsigned int>(a) + b); }
__forceinline int PopValue(SecondaryStack *stack, char requested, void *output)
{
    int top = Add(stack->top, -4);
    if (top < 0) return -1;
    stack->top = top;
    SecondaryValue *value = static_cast<SecondaryValue *>(output);
    value->bits = *reinterpret_cast<DWORD *>(stack->data + top);
    if (requested)
    {
        stack->top = Add(stack->top, -4);
        char type = static_cast<char>(stack->data[stack->top]);
        if (type != 'f')
        {
            if (type == 'i' && requested == 'f') value->real = static_cast<float>(value->integer);
        }
        else if (requested == 'i') value->integer = static_cast<int>(value->real);
    }
    return 0;
}
__forceinline bool PopRaw(SecondaryStack *stack, void *output)
{ return PopValue(stack, 0, output) == 0; }
__forceinline bool PopInt(SecondaryStack *stack, int *output)
{ return PopValue(stack, 'i', output) == 0; }
__forceinline bool PopFloat(SecondaryStack *stack, float *output)
{ return PopValue(stack, 'f', output) == 0; }
__forceinline void PushRaw(SecondaryStack *stack, DWORD value)
{
    if (Add(stack->top, 4) < 4096)
    {
        *reinterpret_cast<DWORD *>(stack->data + stack->top) = value;
        stack->top = Add(stack->top, 4);
    }
}
__forceinline void PushInt(SecondaryStack *stack, int value)
{ PushSecondaryValue(stack, 'i', &value); }
__forceinline void PushFloat(SecondaryStack *stack, float value)
{ PushSecondaryValue(stack, 'f', &value); }
}

void *SecondaryStack::RelativeAddress(int offset)
{ return data + (frame + offset); }
void *__stdcall SecondaryStackAbsoluteAddress(SecondaryStack *self, int offset)
{ return self->data + offset; }
int __stdcall SecondaryStackTop(SecondaryStack *self) { return self->top; }
void SecondaryStack::SetTop(int offset) { top = offset; }
SecondaryProgramView *__stdcall SecondaryScriptTable(SecondaryManager *self) { return self->program; }

int __stdcall PushSecondaryValue(SecondaryStack *self, char type, const void *value)
{
    if (Add(self->top, 4) >= 4096) return -1;
    if (type)
    {
        self->data[self->top] = type;
        self->top = Add(self->top, 4);
    }
    memcpy(self->data + self->top, value, sizeof(DWORD));
    self->top = Add(self->top, 4);
    return 0;
}
int __stdcall PopSecondaryValue(SecondaryStack *self, char type, void *value)
{
    return PopValue(self, type, value);
}
int __stdcall BeginSecondaryStackFrame(SecondaryStack *self, int bytes)
{
    int previous = self->top;
    int top = Add(previous, bytes);
    if (top >= 4096) return -1;
    self->top = top;
    PushRaw(self, self->frame);
    self->frame = previous;
    return 0;
}
int __stdcall EndSecondaryStackFrame(SecondaryStack *self)
{
    int top = self->top;
    top += -4;
    int previous = self->frame;
    if (top >= 0)
    {
        self->top = top;
        self->frame = *reinterpret_cast<int *>(self->data + top);
    }
    self->top = previous;
    return 0;
}
int __stdcall ReadSecondaryIntReference(SecondaryVm *self, int reference)
{
    if (reference >= 0)
        return *reinterpret_cast<int *>(self->stack.data + self->stack.frame + reference);
    if (reference != -1) return self->owner->ReadGlobalInt(reference);
    int result = reference;
    PopInt(&self->stack, &result);
    return result;
}
float __stdcall ReadSecondaryFloatReference(SecondaryVm *self, float reference)
{
    if (reference >= 0)
        return *reinterpret_cast<float *>(self->stack.data + self->stack.frame + static_cast<int>(reference));
    if (reference != -1.0f) return self->owner->ReadGlobalFloat(static_cast<int>(reference));
    float result = reference;
    PopFloat(&self->stack, &result);
    return result;
}
int __stdcall ReadSecondaryIntArgument(SecondaryVm *self, int index)
{
    int reference = Argument(self, index)->integer;
    if (!(self->instruction->references & ReferenceMask(index))) return reference;
    if (reference >= 0)
        return *reinterpret_cast<int *>(self->stack.data + self->stack.frame + reference);
    if (reference != -1) return self->owner->ReadGlobalInt(reference);
    int result = index;
    PopInt(&self->stack, &result);
    return result;
}
float __stdcall ReadSecondaryFloatArgument(SecondaryVm *self, int index)
{
    float reference = Argument(self, index)->real;
    if (!(self->instruction->references & ReferenceMask(index))) return reference;
    if (reference >= 0)
        return *reinterpret_cast<float *>(self->stack.data + self->stack.frame + static_cast<int>(reference));
    if (reference != -1.0f) return self->owner->ReadGlobalFloat(static_cast<int>(reference));
    SecondaryValue result; result.integer = index;
    PopFloat(&self->stack, &result.real);
    return result.real;
}
int *__stdcall SecondaryIntDestination(SecondaryVm *self)
{
    if (!(self->instruction->references & 1)) return 0;
    int reference = Argument(self, 0)->integer;
    if (reference >= 0)
        return reinterpret_cast<int *>(self->stack.data + self->stack.frame + reference);
    return self->owner->GlobalIntPointer(reference);
}
float *__stdcall SecondaryFloatDestination(SecondaryVm *self, int index)
{
    if (!(self->instruction->references & ReferenceMask(index))) return 0;
    float reference = Argument(self, index)->real;
    if (reference >= 0)
        return reinterpret_cast<float *>(self->stack.data + self->stack.frame + static_cast<int>(reference));
    return self->owner->GlobalFloatPointer(static_cast<int>(reference));
}
SecondaryInstruction *__stdcall FindSecondarySubroutine(SecondaryProgramView *self, const char *name)
{
    int low = 0, high = self->subroutineCount - 1;
    while (low <= high)
    {
        int middle = (high - low) / 2 + low;
        int order = strcmp(name, self->subroutines[middle].name);
        if (!order) return reinterpret_cast<SecondaryInstruction *>(self->subroutines[middle].body + 16);
        if (order < 0) high = middle - 1;
        else low = middle + 1;
    }
    return 0;
}
int __stdcall EnterSecondaryScriptFrame(SecondaryVm *self, SecondaryVm *caller, int skip)
{
    int savedTop = self->stack.top;
    int offset = Add(Argument(caller, 0)->integer, 4 + skip * 4);
    DWORD savedFrame = 0;
    int destination = savedTop + 8;
    if (!savedTop)
    {
        PushRaw(&self->stack, 0);
        destination = 12;
    }
    DWORD *output = reinterpret_cast<DWORD *>(self->stack.data + destination);
    for (int i = skip + 1; i < caller->instruction->arguments; ++i, offset += 8, ++output)
    {
        unsigned char *payload = Payload(caller->instruction);
        char sourceType = payload[offset], destinationType = payload[offset + 1];
        SecondaryValue value = *reinterpret_cast<SecondaryValue *>(payload + ((offset + 4) & ~3));
        if (sourceType == 'f' || sourceType == 'g')
        {
            if (caller->instruction->references & ReferenceMask(i))
                value.real = ReadSecondaryFloatReference(caller, value.real);
            if (destinationType == 'f') *reinterpret_cast<float *>(output) = value.real;
            else *reinterpret_cast<int *>(output) = static_cast<int>(value.real);
        }
        else
        {
            if (caller->instruction->references & ReferenceMask(i))
                value.integer = ReadSecondaryIntReference(caller, value.integer);
            if (destinationType == 'f') *reinterpret_cast<float *>(output) = static_cast<float>(value.integer);
            else *output = value.bits;
        }
    }
    if (savedTop)
    {
        PopRaw(&self->stack, &savedFrame);
        self->stack.top = savedTop;
        *reinterpret_cast<DWORD *>(self->stack.data + savedTop - 4) = savedFrame;
        SecondaryValue time; time.real = caller->time;
        PushRaw(&self->stack, time.bits);
        PushRaw(&self->stack, reinterpret_cast<DWORD>(caller->instruction));
    }
    else
    {
        self->stack.top = 4;
        PushRaw(&self->stack, 0);
        PushRaw(&self->stack, 0);
    }
    SecondaryManager *owner = caller->owner;
    SecondaryVm *previous = owner->current;
    owner->current = self;
    self->instruction = FindSecondarySubroutine(owner->program,
        reinterpret_cast<const char *>(Payload(caller->instruction) + 4));
    self->time = 0;
    if (!self->instruction)
    {
        TraceSecondaryScript(Payload(caller->instruction) + 4);
        caller->instruction = 0;
        return -1;
    }
    owner->current = previous;
    return 0;
}
void __stdcall StartSecondaryThread(SecondaryManager *self, int id, int skip)
{
    SecondaryVm *vm = new SecondaryVm;
    SecondaryThread *node = new SecondaryThread;
    vm->time = 0;
    vm->id = id;
    vm->owner = self;
    vm->instruction = 0;
    vm->difficulty = self->current->difficulty;
    node->vm = vm;
    node->next = node->previous = 0;
    if (self->threads.next)
    {
        node->next = self->threads.next;
        self->threads.next->previous = node;
    }
    self->threads.next = node;
    node->previous = &self->threads;
    EnterSecondaryScriptFrame(vm, self->current, skip);
}
SecondaryThread *__stdcall FindSecondaryThread(SecondaryManager *self, int id)
{
    for (SecondaryThread *node = &self->threads; node; node = node->next)
        if (node->vm->id == id) return node;
    return 0;
}
void __stdcall StopSecondaryThreads(SecondaryManager *self)
{
    SecondaryThread *node = self->threads.next;
    while (node)
    {
        SecondaryThread *next = node->next;
        node->vm->instruction = 0;
        node = next;
    }
}
int __stdcall UpdateSecondaryScripts(SecondaryManager *self, float delta)
{
    bool first = true;
    for (SecondaryThread *node = &self->threads; node;)
    {
        SecondaryThread *next = node->next;
        self->current = node->vm;
        int result = RunSecondaryScript(self->current, delta);
        if (first)
        {
            if (result) return -1;
            first = false;
        }
        else if (result)
        {
            delete self->current;
            if (node->next) node->next->previous = node->previous;
            if (node->previous) node->previous->next = node->next;
            node->next = node->previous = 0;
            delete node;
        }
        node = next;
    }
    self->current = &self->main;
    return 0;
}

namespace
{

// Each operation owns its operand temporaries, as in the native dispatch arms.
template<int Op> __forceinline void IntegerOperation(SecondaryStack *stack)
{
    int right, left, result;
    PopInt(stack, &right);
    PopInt(stack, &left);
    if (Op == 50) result = Add(left, right);
    else if (Op == 52) result = static_cast<int>(static_cast<unsigned int>(left) - right);
    else if (Op == 54) result = static_cast<int>(static_cast<unsigned int>(left) * right);
    else if (Op == 56) result = left / right;
    else if (Op == 58) result = left % right;
    else if (Op == 59) result = left == right;
    else if (Op == 61) result = left != right;
    else if (Op == 63) result = left < right;
    else if (Op == 65) result = left <= right;
    else if (Op == 67) result = left > right;
    else if (Op == 69) result = left >= right;
    else if (Op == 73) result = left || right;
    else if (Op == 74) result = left && right;
    else if (Op == 75) result = left ^ right;
    else if (Op == 76) result = left | right;
    else result = left & right;
    PushInt(stack, result);
}
template<int Op> __forceinline void FloatOperation(SecondaryStack *stack)
{
    float right, left;
    PopFloat(stack, &right);
    PopFloat(stack, &left);
    if (Op == 51) PushFloat(stack, left + right);
    else if (Op == 53) PushFloat(stack, left - right);
    else if (Op == 55) PushFloat(stack, left * right);
    else if (Op == 57) PushFloat(stack, left / right);
    else if (Op == 60) PushInt(stack, left == right);
    else if (Op == 62) PushInt(stack, left != right);
    else if (Op == 64) PushInt(stack, left < right);
    else if (Op == 66) PushInt(stack, left <= right);
    else if (Op == 68) PushInt(stack, left > right);
    else PushInt(stack, left >= right);
}
// The target's disabled trace entry is still called with the actual argument types.
__forceinline void TraceInstruction(SecondaryVm *self)
{
    const char *cursor = reinterpret_cast<const char *>(Payload(self->instruction) + 4);
    char *scratch = static_cast<char *>(malloc(1024));
    scratch[0] = 0;
    TraceSecondaryScript();
    if (cursor)
    {
        int argument = 1, typeOffset = 0, valueWord = 6;
        do
        {
            const char *percent = strchr(cursor, '%');
            if (!percent) { TraceSecondaryScript(); break; }
            strcpy(scratch, cursor);
            scratch[percent - cursor] = 0;
            TraceSecondaryScript();
            char format = percent[1];
            if (format == '%') TraceSecondaryScript();
            else if (format == 'd' || format == 'f')
            {
                unsigned char *payload = Payload(self->instruction);
                int length = *reinterpret_cast<int *>(payload);
                char type = payload[length + 4 + typeOffset];
                SecondaryValue value = reinterpret_cast<SecondaryValue *>(self->instruction)[length / 4 + valueWord];
                bool real = type == 'f' || type == 'g';
                if (self->instruction->references & ReferenceMask(argument))
                {
                    if (real) value.real = ReadSecondaryFloatReference(self, value.real);
                    else value.integer = ReadSecondaryIntReference(self, value.integer);
                }
                if (format == 'd') TraceSecondaryScript(real ? static_cast<int>(value.real) : value.integer);
                else TraceSecondaryScript(real ? static_cast<double>(value.real) : static_cast<double>(value.integer));
                typeOffset += 8;
                valueWord += 2;
                ++argument;
            }
            cursor = percent + 2;
        } while (cursor);
    }
    TraceSecondaryScript();
    free(scratch);
}
}

int __stdcall RunSecondaryScript(SecondaryVm *self, float delta)
{
    if (!self->instruction) return -1;
    while (static_cast<double>(self->instruction->time) <= self->time)
    {
        SecondaryInstruction *instruction = self->instruction;
        int opcode = instruction->opcode;
        bool advance = true;
        if (self->difficulty & instruction->difficulty)
        {
            switch (opcode)
            {
            case 0: break;
            case 1: self->instruction = 0; return -1;
            case 10:
                EndSecondaryStackFrame(&self->stack);
                if (!self->stack.top) { self->instruction = 0; return -1; }
                PopRaw(&self->stack, &self->instruction);
                PopRaw(&self->stack, &self->time);
                if (!self->instruction) return -1;
                break;
            case 11:
                if (EnterSecondaryScriptFrame(self, self, 0)) return -1;
                advance = false;
                break;
            case 12:
                self->time = static_cast<float>(Argument(self, 1)->integer);
                self->instruction = reinterpret_cast<SecondaryInstruction *>(
                    reinterpret_cast<unsigned char *>(self->instruction) + Argument(self, 0)->bits);
                advance = false;
                break;
            case 13:
            {
                int condition; PopInt(&self->stack, &condition);
                if (!condition)
                {
                    self->time = static_cast<float>(Argument(self, 1)->integer);
                    self->instruction = reinterpret_cast<SecondaryInstruction *>(
                        reinterpret_cast<unsigned char *>(self->instruction) + Argument(self, 0)->bits);
                    advance = false;
                }
                break;
            }
            case 14:
            {
                int condition; PopInt(&self->stack, &condition);
                if (condition)
                {
                    self->time = static_cast<float>(Argument(self, 1)->integer);
                    self->instruction = reinterpret_cast<SecondaryInstruction *>(
                        reinterpret_cast<unsigned char *>(self->instruction) + Argument(self, 0)->bits);
                    advance = false;
                }
                break;
            }
            case 15: StartSecondaryThread(self->owner, -1, 0); break;
            case 16:
            {
                unsigned int offset = Argument(self, 0)->bits + 4;
                int id = *reinterpret_cast<int *>(Payload(self->instruction) + (offset & ~3u));
                if (self->instruction->references & 2) id = ReadSecondaryIntReference(self, id);
                StartSecondaryThread(self->owner, id, 1);
                break;
            }
            case 17:
            {
                int id = ReadSecondaryIntArgument(self, 0);
                SecondaryThread *node = FindSecondaryThread(self->owner, id);
                if (node) node->vm->instruction = 0;
                break;
            }
            case 18:
            {
                int id = ReadSecondaryIntArgument(self, 0);
                SecondaryThread *node = FindSecondaryThread(self->owner, id);
                if (node) node->vm->flags |= 1;
                break;
            }
            case 19:
            {
                int id = ReadSecondaryIntArgument(self, 0);
                SecondaryThread *node = FindSecondaryThread(self->owner, id);
                if (node) node->vm->flags &= ~1u;
                break;
            }
            case 20:
            {
                int id = ReadSecondaryIntArgument(self, 0);
                SecondaryThread *node = FindSecondaryThread(self->owner, id);
                if (node)
                {
                    SecondaryVm *other = node->vm;
                    other->unknown1018 = ReadSecondaryIntArgument(self, 1);
                }
                break;
            }
            case 21: StopSecondaryThreads(self->owner); break;
            case 30: TraceInstruction(self); break;
            case 40: BeginSecondaryStackFrame(&self->stack, ReadSecondaryIntArgument(self, 0)); break;
            case 41: EndSecondaryStackFrame(&self->stack); break;
            case 42: PushInt(&self->stack, ReadSecondaryIntArgument(self, 0)); break;
            case 43:
            {
                int *destination = SecondaryIntDestination(self);
                SecondaryValue value;
                if (PopRaw(&self->stack, &value))
                {
                    *destination = value.integer;
                    self->stack.top = Add(self->stack.top, -4);
                    if (self->stack.data[self->stack.top] == 'f')
                        { SecondaryValue current; current.integer = *destination; *destination = static_cast<int>(current.real); }
                }
                break;
            }
            case 44: PushFloat(&self->stack, ReadSecondaryFloatArgument(self, 0)); break;
            case 45:
            {
                float *destination = SecondaryFloatDestination(self, 0);
                SecondaryValue value;
                if (PopRaw(&self->stack, &value))
                {
                    *destination = value.real;
                    self->stack.top = Add(self->stack.top, -4);
                    if (self->stack.data[self->stack.top] == 'i')
                        { SecondaryValue current; current.real = *destination; *destination = static_cast<float>(current.integer); }
                }
                break;
            }
            case 50: IntegerOperation<50>(&self->stack); break;
            case 52: IntegerOperation<52>(&self->stack); break;
            case 54: IntegerOperation<54>(&self->stack); break;
            case 56: IntegerOperation<56>(&self->stack); break;
            case 58: IntegerOperation<58>(&self->stack); break;
            case 59: IntegerOperation<59>(&self->stack); break;
            case 61: IntegerOperation<61>(&self->stack); break;
            case 63: IntegerOperation<63>(&self->stack); break;
            case 65: IntegerOperation<65>(&self->stack); break;
            case 67: IntegerOperation<67>(&self->stack); break;
            case 69: IntegerOperation<69>(&self->stack); break;
            case 73: IntegerOperation<73>(&self->stack); break;
            case 74: IntegerOperation<74>(&self->stack); break;
            case 75: IntegerOperation<75>(&self->stack); break;
            case 76: IntegerOperation<76>(&self->stack); break;
            case 77: IntegerOperation<77>(&self->stack); break;
            case 51: FloatOperation<51>(&self->stack); break;
            case 53: FloatOperation<53>(&self->stack); break;
            case 55: FloatOperation<55>(&self->stack); break;
            case 57: FloatOperation<57>(&self->stack); break;
            case 60: FloatOperation<60>(&self->stack); break;
            case 62: FloatOperation<62>(&self->stack); break;
            case 64: FloatOperation<64>(&self->stack); break;
            case 66: FloatOperation<66>(&self->stack); break;
            case 68: FloatOperation<68>(&self->stack); break;
            case 70: FloatOperation<70>(&self->stack); break;
            case 71:
            {
                int value; PopInt(&self->stack, &value);
                PushInt(&self->stack, value == 0); break;
            }
            case 72:
            {
                float value; PopFloat(&self->stack, &value);
                PushInt(&self->stack, value == 0.0f); break;
            }
            case 78:
            {
                int value = ReadSecondaryIntArgument(self, 0);
                *SecondaryIntDestination(self) = Add(value, -1);
                PushInt(&self->stack, value);
                break;
            }
            case 79:
            {
                float value; PopFloat(&self->stack, &value);
                float result = SecondaryUnaryMathA(value);
                PushFloat(&self->stack, result);
                break;
            }
            case 80:
            {
                float value; PopFloat(&self->stack, &value);
                float result = SecondaryUnaryMathB(value);
                PushFloat(&self->stack, result);
                break;
            }
            case 88:
            {
                float value; PopFloat(&self->stack, &value);
                float result = AnimationDistanceRoot(value);
                PushFloat(&self->stack, result);
                break;
            }
            case 81:
            {
                float magnitude = ReadSecondaryFloatArgument(self, 3);
                float angle = NormalizeAngle(ReadSecondaryFloatArgument(self, 2));
                SecondaryVector2 vector;
                vector.SetPolar(angle, magnitude);
                *SecondaryFloatDestination(self, 0) = vector.x;
                *SecondaryFloatDestination(self, 1) = vector.y;
                break;
            }
            case 82:
            {
                float result = NormalizeAngle(ReadSecondaryFloatArgument(self, 0));
                *SecondaryFloatDestination(self, 0) = result;
                break;
            }
            case 83:
            {
                double previous = self->time;
                int amount = ReadSecondaryIntArgument(self, 0);
                self->time = static_cast<float>(previous - amount);
                break;
            }
            case 84:
            {
                int value; PopInt(&self->stack, &value);
                PushInt(&self->stack, static_cast<int>(0u - static_cast<unsigned int>(value)));
                break;
            }
            case 85:
            {
                SecondaryValue value; PopFloat(&self->stack, &value.real);
                value.bits = 0u - value.bits;
                PushSecondaryValue(&self->stack, 'f', &value);
                break;
            }
            case 86:
            {
                float x = ReadSecondaryFloatArgument(self, 1);
                float y = ReadSecondaryFloatArgument(self, 2);
                float *destination = SecondaryFloatDestination(self, 0);
                *destination = x * x + y * y;
                break;
            }
            case 87:
            {
                double endX = ReadSecondaryFloatArgument(self, 2);
                double endY = ReadSecondaryFloatArgument(self, 3);
                float x = static_cast<float>(endX - ReadSecondaryFloatArgument(self, 0));
                float y = static_cast<float>(endY - ReadSecondaryFloatArgument(self, 1));
                float result = SecondaryAngleMath(y, x);
                *SecondaryFloatDestination(self, 0) = result;
                break;
            }
            default:
                if (self->owner->ExecuteInstruction() == -1) return 0;
                break;
            }
        }
        if (advance)
            self->instruction = reinterpret_cast<SecondaryInstruction *>(
                reinterpret_cast<unsigned char *>(self->instruction) + self->instruction->bytes);
    }
    self->time += delta;
    return 0;
}
