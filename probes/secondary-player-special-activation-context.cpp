#include "../src/SecondaryPlayerController.hpp"
extern "C" int __stdcall SecondarySpecialActivationProbe()
{
    return ActivateSecondaryPlayerSpecial();
}
