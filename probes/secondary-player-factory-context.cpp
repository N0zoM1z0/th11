#include "SecondaryPlayerController.hpp"
SecondaryPlayerControllerView *__cdecl CreateSecondaryPlayerController();
extern "C" SecondaryPlayerControllerView *__cdecl SecondaryPlayerFactoryProbe()
{
    return CreateSecondaryPlayerController();
}
