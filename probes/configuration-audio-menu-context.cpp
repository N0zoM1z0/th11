#include "ConfigurationAudioMenu.hpp"

// Compile-only general-input caller. This is not a runnable game entry point.
extern "C" int __stdcall ConfigurationAudioMenuProbe(ConfigurationAudioMenuView *self)
{
    return UpdateConfigurationAudioMenu(self);
}
