#include "JoystickButtons.hpp"
#include "InputBackend.hpp"
#include <MMSystem.h>
#include <string.h>

namespace
{
struct __declspec(align(8)) JoystickButtonStateView
{
    DWORD unknown00[12];
    BYTE buttons[128];
    BYTE unknownb0[0x60];
};
typedef char JoystickButtonStateSize[(sizeof(JoystickButtonStateView) == 0x110) ? 1 : -1];
const HRESULT kInputLost = static_cast<HRESULT>(0x8007001e);
}

// TH11 0x004575A0..0x004576AA. Unlike gameplay mask polling, this publishes
// the individual joystick buttons for input configuration.
unsigned char *PollJoystickButtons()
{
    JOYINFOEX info;
    JoystickButtonStateView state;
    memset(g_JoystickButtonState, 0, sizeof(g_JoystickButtonState));
    if ((g_InputRuntimeFlags & 0x800) == 0)
    {
        memset(&info, 0, sizeof(info));
        info.dwSize = sizeof(info);
        info.dwFlags = 0xff;
        if (joyGetPosEx(0, &info) == 0)
        {
            for (unsigned int index = 0; index < 32; ++index)
            {
                if (info.dwButtons & 1)
                    g_JoystickButtonState[index] = 0x80;
                info.dwButtons >>= 1;
            }
        }
    }
    else if (FAILED(g_JoystickDevice->vtable->Poll(g_JoystickDevice)))
    {
        int attempts = 0;
        HRESULT result = g_JoystickDevice->vtable->Acquire(g_JoystickDevice);
        while (result == kInputLost)
        {
            result = g_JoystickDevice->vtable->Acquire(g_JoystickDevice);
            if (++attempts >= 400)
                break;
        }
    }
    else
    {
        // The target neither clears this buffer nor checks GetDeviceState's
        // HRESULT before copying its 128 button bytes.
        g_JoystickDevice->vtable->GetDeviceState(g_JoystickDevice, sizeof(state), &state);
        memcpy(g_JoystickButtonState, state.buttons, sizeof(g_JoystickButtonState));
    }
    return g_JoystickButtonState;
}
