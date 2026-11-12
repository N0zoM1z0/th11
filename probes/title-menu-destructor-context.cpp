#include "TitleMenuLifecycle.hpp"
extern "C" void __stdcall TitleMenuDestructorProbe(TitleMenuView *menu)
{
    menu->~TitleMenuView();
}
