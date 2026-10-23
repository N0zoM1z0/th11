#pragma once

#include "OutlinedText.hpp"
#include <stddef.h>

struct TextTextureBindingView
{
    IDirect3DTexture9 *texture;
};

struct TextTextureAttachmentView
{
    unsigned char unknown000[8];
    TextTextureBindingView *binding;
    float left, top, right, bottom;
    unsigned char unknown01c[0x1c];
    float width;
};

struct FormattedTextVmView
{
    unsigned char unknown000[0x3ac];
    TextTextureAttachmentView *attachment;
    unsigned char unknown3b0[0x54];
    unsigned int changed;
    unsigned int textFlags;
    unsigned char unknown40c[0x10];
    unsigned char fontHeight;
};

typedef char TextAttachmentBindingOffset[(offsetof(TextTextureAttachmentView, binding) == 8) ? 1 : -1];
typedef char TextAttachmentWidthOffset[(offsetof(TextTextureAttachmentView, width) == 0x38) ? 1 : -1];
typedef char FormattedTextAttachmentOffset[(offsetof(FormattedTextVmView, attachment) == 0x3ac) ? 1 : -1];
typedef char FormattedTextChangedOffset[(offsetof(FormattedTextVmView, changed) == 0x404) ? 1 : -1];
typedef char FormattedTextFlagsOffset[(offsetof(FormattedTextVmView, textFlags) == 0x408) ? 1 : -1];
typedef char FormattedTextHeightOffset[(offsetof(FormattedTextVmView, fontHeight) == 0x41c) ? 1 : -1];

void __cdecl FormatTextureText(FormattedTextVmView *state, DWORD foreground,
    DWORD outline, int x, int fontChoice, int spacing, const char *format, ...);
void __cdecl FormatRightAlignedTextureText(FormattedTextVmView *state,
    DWORD foreground, DWORD outline, int fontChoice, int spacing, const char *format, ...);
void __cdecl FormatCenteredTextureText(FormattedTextVmView *state,
    DWORD foreground, DWORD outline, int fontChoice, int spacing, const char *format, ...);
