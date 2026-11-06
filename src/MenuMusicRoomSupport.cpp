#include "MenuMusicRoom.hpp"
#include <string.h>

char *__stdcall SkipMenuMusicLine(char *cursor, int *remaining)
{
    while (*cursor != '\n' && *cursor != '\r')
    {
        if (!*remaining) return cursor;
        ++cursor;
        --*remaining;
    }
    if (*remaining)
        while ((*cursor == '\n' || *cursor == '\r') && *remaining)
        {
            ++cursor;
            --*remaining;
        }
    return cursor;
}

char *__stdcall CopyMenuMusicLine(char *cursor, int *remaining, char *destination)
{
    char *end = cursor;
    while (*end != '\n' && *end != '\r')
    {
        if (!*remaining) return end;
        ++end;
        --*remaining;
    }
    if (*remaining)
    {
        *end = 0;
        strcpy(destination, cursor);
        do
        {
            ++end;
            --*remaining;
        } while ((*end == '\n' || *end == '\r') && *remaining);
    }
    return end;
}

void __stdcall TweenMenuMusicPosition(AnimationVmView *vm,
    const CameraVector *target, const CameraVector *initial)
{
    StageVectorTween *tween = reinterpret_cast<StageVectorTween *>(
        reinterpret_cast<unsigned char *>(vm) + 0x90);
    tween->duration = 4;
    tween->control1 = g_MenuMusicTweenControls;
    tween->control2 = g_MenuMusicTweenControls;
    tween->mode = 0;
    tween->initial = *initial;
    tween->target = *target;
    tween->time.SetFrame(-1);
}
