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
    unsigned int unknown018;
    int page, state;
    SecondaryDebugSelectionView selection;
    unsigned char unknown0fc[0x2b0 - 0xfc];
    StageTimerView time;
    unsigned int unknown2c4;
    AnimationVmId rootVmId;
};

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

// Reviewed source-open boundaries. Their private LTCG register assignments are
// observations, not expressed as invented public calling conventions here.
void __stdcall PushConfigurationSelection(SecondaryDebugSelectionView *selection);
void __stdcall PopConfigurationSelection(SecondaryDebugSelectionView *selection);
void __stdcall SignalAndExecuteConfigurationAnimation(AnimationVmId id, short event);
