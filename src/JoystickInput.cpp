#include "InputBackend.hpp"
#include <MMSystem.h>
#include <string.h>

namespace
{
struct JoystickState2View
{
    LONG x, y, z, rx, ry, rz;
    LONG sliders[2];
    DWORD pov[4];
    BYTE buttons[128];
    BYTE unknown0b0[0x60];
};
typedef char JoystickState2Size[(sizeof(JoystickState2View) == 0x110) ? 1 : -1];
const HRESULT kInputLost = static_cast<HRESULT>(0x8007001e);
}

// TH11 0x00457270..0x0045759B receives the existing keyboard mask in ECX.
// The backend failure paths keep that mask; successful reads merge five
// configurable buttons and four strict axis-threshold comparisons.
unsigned int __fastcall MergeJoystickInput(unsigned int buttons)
{
    if ((g_InputRuntimeFlags & 0x800) == 0)
    {
        JOYINFOEX state;
        memset(&state, 0, sizeof(state));
        state.dwSize = sizeof(state);
        state.dwFlags = 0xff;
        if (joyGetPosEx(0, &state) == 0)
        {
            if (g_JoystickBindings.fire >= 0)
                buttons |= (state.dwButtons & (1u << g_JoystickBindings.fire)) != 0;
            if (g_JoystickBindings.bomb >= 0)
                buttons |= (state.dwButtons & (1u << g_JoystickBindings.bomb)) ? 2 : 0;
            if (g_JoystickBindings.pause >= 0)
                buttons |= (state.dwButtons & (1u << g_JoystickBindings.pause)) ? 0x100 : 0;
            if (g_JoystickBindings.slow >= 0)
                buttons |= (state.dwButtons & (1u << g_JoystickBindings.slow)) ? 8 : 0;
            if (g_JoystickBindings.skip >= 0)
                buttons |= (state.dwButtons & (1u << g_JoystickBindings.skip)) ? 0x200 : 0;
            const unsigned int halfX = (g_JoystickAxisLimits.xMax + g_JoystickAxisLimits.xMin) >> 1;
            const unsigned int bandX = (g_JoystickAxisLimits.xMax - g_JoystickAxisLimits.xMin) >> 2;
            const unsigned int halfY = (g_JoystickAxisLimits.yMax + g_JoystickAxisLimits.yMin) >> 1;
            const unsigned int bandY = (g_JoystickAxisLimits.yMax - g_JoystickAxisLimits.yMin) >> 2;
            buttons |= state.dwXpos < halfX - bandX ? 0x40 : 0;
            buttons |= state.dwXpos > halfX + bandX ? 0x80 : 0;
            buttons |= state.dwYpos < halfY - bandY ? 0x10 : 0;
            buttons |= state.dwYpos > halfY + bandY ? 0x20 : 0;
        }
        return buttons;
    }

    if (FAILED(g_JoystickDevice->vtable->Poll(g_JoystickDevice)))
    {
        HRESULT result = g_JoystickDevice->vtable->Acquire(g_JoystickDevice);
        int attempts = 0;
        while (result == kInputLost && attempts < 400)
        {
            result = g_JoystickDevice->vtable->Acquire(g_JoystickDevice);
            InputAcquireHook();
            ++attempts;
        }
        return buttons;
    }

    JoystickState2View state;
    memset(&state, 0, sizeof(state));
    if (SUCCEEDED(g_JoystickDevice->vtable->GetDeviceState(g_JoystickDevice, sizeof(state), &state)))
    {
        if (g_JoystickBindings.fire >= 0)
            buttons |= state.buttons[g_JoystickBindings.fire] >> 7;
        if (g_JoystickBindings.bomb >= 0)
            buttons |= (state.buttons[g_JoystickBindings.bomb] >> 6) & 2;
        if (g_JoystickBindings.pause >= 0)
            buttons |= (state.buttons[g_JoystickBindings.pause] & 0x80) << 1;
        if (g_JoystickBindings.slow >= 0)
            buttons |= (state.buttons[g_JoystickBindings.slow] >> 4) & 8;
        if (g_JoystickBindings.skip >= 0)
            buttons |= (state.buttons[g_JoystickBindings.skip] & 0x80) << 2;
        buttons |= state.x < -static_cast<int>(g_JoystickThresholdX) ? 0x40 : 0;
        buttons |= state.x > g_JoystickThresholdX ? 0x80 : 0;
        buttons |= state.y < -static_cast<int>(g_JoystickThresholdY) ? 0x10 : 0;
        buttons |= state.y > g_JoystickThresholdY ? 0x20 : 0;
    }
    return buttons;
}
