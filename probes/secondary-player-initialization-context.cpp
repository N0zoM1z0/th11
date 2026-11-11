#include "SecondaryPlayerController.hpp"
// Isolated compile-only original player initializer caller.
int __stdcall InitializeSecondaryPlayerController(SecondaryDamageReferencesView *);
extern "C" int __stdcall SecondaryPlayerInitializationProbe(
    SecondaryDamageReferencesView *controller)
{
    return InitializeSecondaryPlayerController(controller);
}
