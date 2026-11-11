#include "SecondaryPlayerController.hpp"
#include <new>

// A physical RAII view of the 0x8D24-byte allocation, deliberately not
// claiming the original game's nominal class or complete C++ hierarchy.
SecondaryPlayerControllerView *__stdcall ConstructSecondaryPlayerStorage(SecondaryPlayerControllerView *);
void __stdcall DestructSecondaryPlayerStorage(SecondaryPlayerControllerView *);
int __stdcall InitializeSecondaryPlayerController(SecondaryDamageReferencesView *);

// The native factory has a real C++ EH/unwind state. Binding construction
// and destruction to this object naturally restores that exception frame
// without inserting artificial SEH instructions or dummy locals.
struct SecondaryPlayerStorageObject
{
    unsigned char storage[sizeof(SecondaryPlayerControllerView)];
    SecondaryPlayerStorageObject()
    {
        ConstructSecondaryPlayerStorage(reinterpret_cast<SecondaryPlayerControllerView *>(this));
    }
    ~SecondaryPlayerStorageObject()
    {
        DestructSecondaryPlayerStorage(reinterpret_cast<SecondaryPlayerControllerView *>(this));
    }
};
typedef char SecondaryPlayerRAIISize[
    (sizeof(SecondaryPlayerStorageObject) == 0x8d24) ? 1 : -1];

SecondaryPlayerControllerView *__cdecl CreateSecondaryPlayerController()
{
    SecondaryPlayerStorageObject *owner = new SecondaryPlayerStorageObject;
    if (InitializeSecondaryPlayerController(
            reinterpret_cast<SecondaryDamageReferencesView *>(owner)))
    {
        delete owner;
        return 0;
    }
    return reinterpret_cast<SecondaryPlayerControllerView *>(owner);
}
