#include "FormattedTextureText.hpp"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

void __cdecl FormatTextureText(FormattedTextVmView *state, DWORD foreground,
    DWORD outline, int x, int fontChoice, int spacing, const char *format, ...)
{
    char buffer[132];
    unsigned int height = state->fontHeight;
    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);
    TextTextureAttachmentView *attachment = state->attachment;
    DrawPlacedTextureText(attachment->binding->texture,
        reinterpret_cast<const TextTexturePlacementView *>(attachment), x,
        foreground, outline, (state->textFlags >> 1) & 1, spacing,
        height, buffer, fontChoice);
    state->changed |= 1;
}

void __cdecl FormatRightAlignedTextureText(FormattedTextVmView *state,
    DWORD foreground, DWORD outline, int fontChoice, int spacing, const char *format, ...)
{
    char buffer[132];
    unsigned int height = 17;
    if (state->fontHeight > 0)
        height = state->fontHeight;
    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);
    unsigned int length = strlen(buffer);
    TextTextureAttachmentView *attachment = state->attachment;
    DrawPlacedTextureText(attachment->binding->texture,
        reinterpret_cast<const TextTexturePlacementView *>(attachment),
        static_cast<int>(attachment->width - (((height - 1) * length) >> 1)),
        foreground, outline, (state->textFlags >> 1) & 1, spacing,
        height, buffer, fontChoice);
    state->changed |= 1;
}

void __cdecl FormatCenteredTextureText(FormattedTextVmView *state,
    DWORD foreground, DWORD outline, int fontChoice, int spacing, const char *format, ...)
{
    char buffer[132];
    unsigned int height = 17;
    if (state->fontHeight > 0)
        height = state->fontHeight;
    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);
    unsigned int length = strlen(buffer);
    TextTextureAttachmentView *attachment = state->attachment;
    DrawPlacedTextureText(attachment->binding->texture,
        reinterpret_cast<const TextTexturePlacementView *>(attachment),
        static_cast<int>(attachment->width) / 2 - (((height - 1) * length) >> 2),
        foreground, outline, (state->textFlags >> 1) & 1, spacing,
        height, buffer, fontChoice);
    state->changed |= 1;
}
