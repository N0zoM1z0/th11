#include "SecondaryPlayerController.hpp"
#include <new>

// Native TH11 004301B0..00430247: allocation/initialization factory.
// These storage lifetime helpers are the independent original 42F760
// and 430010 owners, not placeholder copies of their code.
SecondaryPlayerControllerView *__stdcall ConstructSecondaryPlayerStorage(
    SecondaryPlayerControllerView *memory);
void __stdcall DestructSecondaryPlayerStorage(SecondaryPlayerControllerView *owner);
int __stdcall InitializeSecondaryPlayerController(SecondaryDamageReferencesView *self);

typedef char SecondaryControllerAllocationSize[
    (sizeof(SecondaryPlayerControllerView) == 0x8d24) ? 1 : -1];

SecondaryPlayerControllerView *__cdecl CreateSecondaryPlayerController()
{
    SecondaryPlayerControllerView *memory = static_cast<SecondaryPlayerControllerView *>(
        ::operator new(sizeof(SecondaryPlayerControllerView)));
    SecondaryPlayerControllerView *owner =
        memory ? ConstructSecondaryPlayerStorage(memory) : 0;
    if (InitializeSecondaryPlayerController(
        reinterpret_cast<SecondaryDamageReferencesView *>(owner)))
    {
        if (owner)
        {
            DestructSecondaryPlayerStorage(owner);
            ::operator delete(owner);
        }
        return 0;
    }
    return owner;
}
