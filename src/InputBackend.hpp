#pragma once
#include <Windows.h>
#include "InputState.hpp"

// Partial DirectInput COM vtable; only observed methods are named.
struct DirectInputDeviceView;
struct DirectInputDeviceVtableView
{
    void *unknown00[7];
    HRESULT (__stdcall *Acquire)(DirectInputDeviceView *);
    void *unknown20;
    HRESULT (__stdcall *GetDeviceState)(DirectInputDeviceView *, DWORD, void *);
    void *unknown28[15];
    HRESULT (__stdcall *Poll)(DirectInputDeviceView *);
};
struct DirectInputDeviceView
{
    DirectInputDeviceVtableView *vtable;
};

extern unsigned int g_InputRuntimeFlags;
extern int g_KeyboardPollingEnabled;
extern DirectInputDeviceView *g_KeyboardDevice;
extern InputStateView g_InputState;

unsigned int __fastcall MergeJoystickInput(unsigned int buttons);
unsigned int PollKeyboardInput();

struct JoystickAxisLimitsView
{
    unsigned int xMin, xMax, yMin, yMax;
};
struct JoystickBindingsView
{
    short fire, bomb, slow, pause;
    short unknown08[4];
    short skip;
};
extern DirectInputDeviceView *g_JoystickDevice;
extern JoystickAxisLimitsView g_JoystickAxisLimits;
extern JoystickBindingsView g_JoystickBindings;
extern short g_JoystickThresholdX;
extern short g_JoystickThresholdY;
void InputAcquireHook(const char *format, ...);
