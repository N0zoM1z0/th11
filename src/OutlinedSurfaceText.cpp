#include "OutlinedText.hpp"

bool __stdcall DrawOutlinedSurfaceText(TextSurfaceView *state, const char *text,
    int x, int y, HFONT font, COLORREF foreground, COLORREF outline)
{
    if (!text)
        return false;
    int width = state->width;
    int height = state->height;
    HDC dc = state->dc;
    HGDIOBJ previous = SelectObject(dc, font);
    SetBkMode(dc, TRANSPARENT);
    RECT first, main, second, third, fourth, fifth, sixth;
    if (outline != CLR_INVALID)
    {
        SetTextColor(dc, outline);
        first.left = x;
        first.top = y;
        first.right = x + width - 2;
        first.bottom = y + height - 2;
        DrawTextA(dc, text, -1, &first, 0);
        second.left = x + 2;
        second.top = y;
        second.right = x + width + 2;
        second.bottom = y + height;
        third.left = x + 1;
        third.top = y + 2;
        third.right = x + width + 1;
        third.bottom = y + height + 2;
        fourth.left = x + 2;
        fourth.top = y;
        fourth.right = x + width + 2;
        fourth.bottom = y + height;
        fifth.left = x;
        fifth.top = y + 2;
        fifth.right = x + width;
        fifth.bottom = y + height + 2;
        sixth.left = x + 2;
        sixth.top = y + 2;
        sixth.right = x + width + 2;
        sixth.bottom = y + height + 2;
        DrawTextA(dc, text, -1, &second, 0);
        DrawTextA(dc, text, -1, &third, 0);
        DrawTextA(dc, text, -1, &fourth, 0);
        DrawTextA(dc, text, -1, &fifth, 0);
        DrawTextA(dc, text, -1, &sixth, 0);
    }
    SetTextColor(dc, foreground);
    main.left = x + 1;
    main.top = y + 1;
    main.right = width + 1 + x;
    main.bottom = height + 1 + y;
    DrawTextA(dc, text, -1, &main, 0);
    SelectObject(dc, previous);
    return true;
}
