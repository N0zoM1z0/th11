#include "FormattedTextureText.hpp"

extern "C" void __stdcall FormattedTextProbe(FormattedTextVmView *first,
    FormattedTextVmView *second, DWORD foreground, DWORD outline, int x,
    int fontChoice, int spacing, const char *format, unsigned int value, const char *text)
{
    FormatTextureText(first, foreground, outline, x, fontChoice, spacing, format, value, text);
    FormatRightAlignedTextureText(second, foreground, outline, fontChoice, spacing, format, value, text);
    FormatCenteredTextureText(first, foreground, outline, fontChoice, spacing, format, value, text);
}
