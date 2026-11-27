#include "../src/SecondaryScript.hpp"

// Native PushSecondaryValue keeps its type test and receives the type in DL.
// In a link where every caller passes a constant, VC9 specializes it; this
// independent dynamic caller keeps the real three-parameter interface.
extern "C" __declspec(dllexport) int __stdcall SecondaryScriptPushProbe(SecondaryVm *vm, int type, int *value)
{
    return PushSecondaryValue(&vm->stack, static_cast<char>(type), value);
}
