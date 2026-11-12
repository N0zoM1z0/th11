#include "../src/MenuPracticeStage.hpp"

// The native 0x40FCA0 producer passes a stack-backed XYZ to the position factory.
extern "C" void __stdcall PositionedVmFactoryProbe(float x, float y)
{
    StartMenuTransitionOverlay(x, y);
}
