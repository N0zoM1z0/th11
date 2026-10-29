// Independent VM/manager/program/output inputs. Diagnostic image is never run.
#include "SecondaryScript.hpp"
extern "C" int __stdcall SecondaryScriptProbe(int kind, SecondaryVm *vm,
    SecondaryManager *manager, SecondaryProgramView *program, int offset,
    int *integer, float *real, float delta)
{
    SecondaryValue value;
    switch (kind)
    {
    case 0: return RunSecondaryScript(vm, delta);
    case 1: return UpdateSecondaryScripts(manager, delta);
    case 2: return EnterSecondaryScriptFrame(vm, manager->current, offset);
    case 3: return reinterpret_cast<int>(vm->stack.RelativeAddress(offset));
    case 4: return reinterpret_cast<int>(SecondaryStackAbsoluteAddress(&vm->stack, offset));
    case 5: return SecondaryStackTop(&vm->stack);
    case 6: vm->stack.SetTop(offset); break;
    case 7: return reinterpret_cast<int>(SecondaryScriptTable(manager));
    case 8: return PushSecondaryValue(&vm->stack, static_cast<char>(offset), integer);
    case 9: return BeginSecondaryStackFrame(&vm->stack, offset);
    case 10: return EndSecondaryStackFrame(&vm->stack);
    case 11: return ReadSecondaryIntReference(vm, offset);
    case 12: value.real = ReadSecondaryFloatReference(vm, delta); return value.integer;
    case 13: return ReadSecondaryIntArgument(vm, offset);
    case 14: value.real = ReadSecondaryFloatArgument(vm, offset); return value.integer;
    case 15: return reinterpret_cast<int>(SecondaryIntDestination(vm));
    case 16: return reinterpret_cast<int>(SecondaryFloatDestination(vm, offset));
    case 17: return reinterpret_cast<int>(FindSecondarySubroutine(program, reinterpret_cast<const char *>(integer)));
    case 18: StartSecondaryThread(manager, offset, static_cast<int>(delta)); break;
    case 19: return reinterpret_cast<int>(FindSecondaryThread(manager, offset));
    case 20: StopSecondaryThreads(manager); break;
    case 21: return PopSecondaryValue(&vm->stack, static_cast<char>(offset), real);
    }
    return 0;
}
