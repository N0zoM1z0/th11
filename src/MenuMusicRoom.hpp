#pragma once

#include "MenuController.hpp"
#include "StageTween.hpp"

// Additional physical view of the existing menu. The storage spans follow the
// independently observed neighboring fields; the parser does not clamp counts.
struct MenuMusicRoomView
{
    unsigned char unknown000[0x60c];
    AnimationVmId trackVmIds[20];
    AnimationVmId commentVmIds[8];
    AnimationVmId secondaryRootVmId;
    int trackCount, nextCommentLine, commentTrack, warningPending;
    char filenames[32][0x40];
    char titles[32][0x42];
    char comments[32][8][0x42];
    int firstVisibleTrack;
    unsigned char unknown58d4[0x5b6c - 0x58d4];
    unsigned char *textBuffer;
};

typedef char MenuMusicTrackIdsOffset[(offsetof(MenuMusicRoomView, trackVmIds) == 0x60c) ? 1 : -1];
typedef char MenuMusicCommentIdsOffset[(offsetof(MenuMusicRoomView, commentVmIds) == 0x65c) ? 1 : -1];
typedef char MenuMusicCountOffset[(offsetof(MenuMusicRoomView, trackCount) == 0x680) ? 1 : -1];
typedef char MenuMusicFilenamesOffset[(offsetof(MenuMusicRoomView, filenames) == 0x690) ? 1 : -1];
typedef char MenuMusicTitlesOffset[(offsetof(MenuMusicRoomView, titles) == 0xe90) ? 1 : -1];
typedef char MenuMusicCommentsOffset[(offsetof(MenuMusicRoomView, comments) == 0x16d0) ? 1 : -1];
typedef char MenuMusicScrollOffset[(offsetof(MenuMusicRoomView, firstVisibleTrack) == 0x58d0) ? 1 : -1];
typedef char MenuMusicBufferOffset[(offsetof(MenuMusicRoomView, textBuffer) == 0x5b6c) ? 1 : -1];

extern AnimationFileView *g_MenuMusicCommentFile;
extern const char *g_MenuMusicWarningLines[8];
extern const char g_MenuMusicHiddenTitle[];
extern CameraVector g_MenuMusicTweenControls;

char *__stdcall SkipMenuMusicLine(char *cursor, int *remaining);
char *__stdcall CopyMenuMusicLine(char *cursor, int *remaining, char *destination);
void __stdcall TweenMenuMusicPosition(AnimationVmView *vm,
    const CameraVector *target, const CameraVector *initial);
// Reviewed call boundary; the producer's complete body is still source-open.
int __stdcall EnqueueMenuMusicStart(int position, int track);
