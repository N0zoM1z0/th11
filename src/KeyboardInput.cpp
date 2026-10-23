#include "InputBackend.hpp"

namespace
{
__forceinline unsigned int VirtualKeyButtons(const BYTE *keys)
{
    return ((keys['Z'] & 0x80) >> 7) |
        ((keys['X'] & 0x80) >> 6) |
        ((keys[VK_SHIFT] & 0x80) >> 4) |
        (((keys[VK_NUMPAD8] | keys[VK_UP]) & 0x80) >> 3) |
        (((keys[VK_NUMPAD2] | keys[VK_DOWN]) & 0x80) >> 2) |
        (((keys[VK_NUMPAD4] | keys[VK_LEFT]) & 0x80) >> 1) |
        ((keys[VK_NUMPAD6] | keys[VK_RIGHT]) & 0x80) |
        ((keys[VK_ESCAPE] & 0x80) << 1) |
        ((keys[VK_CONTROL] & 0x80) << 2) |
        ((keys['Q'] & 0x80) << 9) |
        ((keys['S'] & 0x80) << 10) |
        (((keys['P'] | keys[VK_HOME]) & 0x80) << 11) |
        ((keys[VK_RETURN] & 0x80) << 12) |
        ((keys['D'] & 0x80) << 13) |
        ((keys['R'] & 0x80) << 14) |
        ((keys[VK_F10] & 0x80) << 16) |
        ((keys[VK_NUMPAD3] & 0x80) ? 0xa0 : 0) |
        ((keys[VK_NUMPAD9] & 0x80) ? 0x90 : 0) |
        ((keys[VK_NUMPAD1] & 0x80) ? 0x60 : 0) |
        ((keys[VK_NUMPAD7] & 0x80) ? 0x50 : 0);
}

__forceinline unsigned int ScanCodeButtons(const BYTE *keys)
{
    return ((keys[0x2c] & 0x80) >> 7) |
        ((keys[0x2d] & 0x80) >> 6) |
        ((keys[0x2a] & 0x80) >> 4) |
        (((keys[0x48] | keys[0xc8]) & 0x80) >> 3) |
        (((keys[0xd0] | keys[0x50]) & 0x80) >> 2) |
        (((keys[0x4b] | keys[0xcb]) & 0x80) >> 1) |
        ((keys[0x4d] | keys[0xcd]) & 0x80) |
        ((keys[0x01] & 0x80) << 1) |
        ((keys[0x1d] & 0x80) << 2) |
        ((keys[0x10] & 0x80) << 9) |
        ((keys[0x1f] & 0x80) << 10) |
        (((keys[0x19] | keys[0xc7]) & 0x80) << 11) |
        ((keys[0x1c] & 0x80) << 12) |
        ((keys[0x20] & 0x80) << 13) |
        ((keys[0x13] & 0x80) << 14) |
        ((keys[0x44] & 0x80) << 16) |
        ((keys[0x49] & 0x80) ? 0x90 : 0) |
        ((keys[0x4f] & 0x80) ? 0x60 : 0) |
        ((keys[0x47] & 0x80) ? 0x50 : 0) |
        ((keys[0x51] & 0x80) ? 0xa0 : 0);
}

}

// TH11 0x004576B0..0x00457A27. Keyboard state is followed by joystick merge,
// publication of previous/current bits, then the shared repeat/edge update.
unsigned int PollKeyboardInput()
{
    BYTE keys[256];
    unsigned int buttons = 0;
    if (g_KeyboardPollingEnabled)
    {
        if ((g_InputRuntimeFlags & 0x400) == 0)
        {
            GetKeyboardState(keys);
            buttons = VirtualKeyButtons(keys);
        }
        else
        {
            HRESULT result = g_KeyboardDevice->vtable->GetDeviceState(g_KeyboardDevice, 256, keys);
            buttons = 0;
            if (result == static_cast<HRESULT>(0x8007001e) || result != 0)
            {
                g_KeyboardDevice->vtable->Acquire(g_KeyboardDevice);
            }
            else
                buttons = ScanCodeButtons(keys);
        }
    }
    buttons = MergeJoystickInput(buttons);
    g_InputState.previous = g_InputState.current;
    g_InputState.current = buttons;
    UpdateInputRepeatState(&g_InputState);
    return buttons;
}
