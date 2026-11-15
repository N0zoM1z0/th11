// Non-runnable link context: the title menu page drives the primary-menu
// helpers; the stage mesh supplies other live callers of the VM factory.
#include "../src/MenuController.hpp"
extern "C" int __stdcall MenuPrimaryProbe(MenuControllerView *self)
{
    return UpdateMenuPage01(self);
}
