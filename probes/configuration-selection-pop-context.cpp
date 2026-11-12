#include "ConfigurationAudioMenu.hpp"
void __stdcall PopConfigurationSelection(SecondaryDebugSelectionView *selection);
void __stdcall PushConfigurationSelection(SecondaryDebugSelectionView *selection);
extern "C" void __stdcall ConfigurationSelectionPopProbe(SecondaryDebugSelectionView *selection, int mode)
{
    if (mode) PushConfigurationSelection(selection);
    PopConfigurationSelection(selection);
}
