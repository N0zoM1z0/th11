#pragma once

#include "AnimationVm.hpp"
#include "AnimationVmId.hpp"
#include "SecondaryGameplay.hpp"
#include "StageResource.hpp"

// Partial physical menu view shared by the reviewed audio page and its callers.
// The original class name and complete allocation size are unresolved.
struct ConfigurationAudioMenuView
{
    unsigned char unknown000[0x14];
    AnimationFileView *file;
    AnimationFileView *secondaryFile;
    int page, state;
    SecondaryDebugSelectionView selection;
    unsigned char unknown0fc[0x2b0 - 0xfc];
    StageTimerView time;
    AnimationVmId primaryVmId;
    AnimationVmId rootVmId;
};

typedef char ConfigurationMenuSecondaryFileOffset[(offsetof(ConfigurationAudioMenuView, secondaryFile) == 0x18) ? 1 : -1];
typedef char ConfigurationMenuPrimaryVmOffset[(offsetof(ConfigurationAudioMenuView, primaryVmId) == 0x2c4) ? 1 : -1];
typedef char ConfigurationMenuPageOffset[(offsetof(ConfigurationAudioMenuView, page) == 0x1c) ? 1 : -1];
typedef char ConfigurationMenuSelectionOffset[(offsetof(ConfigurationAudioMenuView, selection) == 0x24) ? 1 : -1];
typedef char ConfigurationMenuTimerOffset[(offsetof(ConfigurationAudioMenuView, time) == 0x2b0) ? 1 : -1];
typedef char ConfigurationMenuVmOffset[(offsetof(ConfigurationAudioMenuView, rootVmId) == 0x2c8) ? 1 : -1];

// Separate observed scalar at 0x004C9170; its other consumers remain open.
extern int g_ConfigurationSoundAttenuation;

void __stdcall ApplyConfigurationAudioMenu(ConfigurationAudioMenuView *self);
int __stdcall UpdateConfigurationAudioMenu(ConfigurationAudioMenuView *self);
void __stdcall SetConfigurationMenuState(ConfigurationAudioMenuView *self, int state);
void __stdcall SetConfigurationMenuPage(ConfigurationAudioMenuView *self, int page);

// Reviewed shared operations, defined in the menu selection/animation sources.
// Private register allocation is left to the real /GL compiler.
void __stdcall PushConfigurationSelection(SecondaryDebugSelectionView *selection);
void __stdcall PopConfigurationSelection(SecondaryDebugSelectionView *selection);
void __stdcall SignalAndExecuteConfigurationAnimation(AnimationVmId id, short event);
