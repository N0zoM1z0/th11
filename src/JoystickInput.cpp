#include "InputBackend.hpp"
#include <MMSystem.h>
#include <string.h>

namespace
{
// The target places this buffer at an 8-byte aligned stack address.
struct __declspec(align(8)) JoystickState2View
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
    JOYINFOEX info;
    JoystickState2View state;
    if ((g_InputRuntimeFlags & 0x800) == 0)
    {
        memset(&info, 0, sizeof(info));
        info.dwSize = sizeof(info);
        info.dwFlags = 0xff;
        if (joyGetPosEx(0, &info) != 0)
            return buttons;
        {
            if (g_JoystickBindings.fire >= 0)
                buttons |= (info.dwButtons & (1u << g_JoystickBindings.fire)) ? 1 : 0;
            if (g_JoystickBindings.bomb >= 0)
                buttons |= (info.dwButtons & (1u << g_JoystickBindings.bomb)) ? 2 : 0;
            if (g_JoystickBindings.pause >= 0)
                buttons |= (info.dwButtons & (1u << g_JoystickBindings.pause)) ? 0x100 : 0;
            if (g_JoystickBindings.slow >= 0)
                buttons |= (info.dwButtons & (1u << g_JoystickBindings.slow)) ? 8 : 0;
            if (g_JoystickBindings.skip >= 0)
                buttons |= (info.dwButtons & (1u << g_JoystickBindings.skip)) ? 0x200 : 0;
            const unsigned int bandX = (g_JoystickAxisLimits.xMax - g_JoystickAxisLimits.xMin) >> 2;
            buttons |= info.dwXpos > ((g_JoystickAxisLimits.xMin + g_JoystickAxisLimits.xMax) >> 1) + bandX ? 0x80 : 0;
            buttons |= info.dwXpos < ((g_JoystickAxisLimits.xMin + g_JoystickAxisLimits.xMax) >> 1) - bandX ? 0x40 : 0;
            const unsigned int bandY = (g_JoystickAxisLimits.yMax - g_JoystickAxisLimits.yMin) >> 2;
            buttons |= info.dwYpos > ((g_JoystickAxisLimits.yMin + g_JoystickAxisLimits.yMax) >> 1) + bandY ? 0x20 : 0;
            buttons |= info.dwYpos < ((g_JoystickAxisLimits.yMin + g_JoystickAxisLimits.yMax) >> 1) - bandY ? 0x10 : 0;
        }
        return buttons;
    }

    if (FAILED(g_JoystickDevice->vtable->Poll(g_JoystickDevice)))
    {
        int attempts = 0;
        HRESULT result = g_JoystickDevice->vtable->Acquire(g_JoystickDevice);
        while (result == kInputLost)
        {
            result = g_JoystickDevice->vtable->Acquire(g_JoystickDevice);
            InputAcquireHook("error : DIERR_INPUTLOST %d\r\n", attempts);
            if (++attempts >= 400)
                return buttons;
        }
        return buttons;
    }

    memset(&state, 0, sizeof(state));
    if (FAILED(g_JoystickDevice->vtable->GetDeviceState(g_JoystickDevice, sizeof(state), &state)))
        return buttons;
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
        buttons |= state.y < -static_cast<int>(g_JoystickThresholdY) ? 0x10 : 0;
        buttons |= state.x > g_JoystickThresholdX ? 0x80 : 0;
        buttons |= state.y > g_JoystickThresholdY ? 0x20 : 0;
    }
    return buttons;
}
