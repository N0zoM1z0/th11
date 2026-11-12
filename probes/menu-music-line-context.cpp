#include "MenuMusicRoom.hpp"
#include "MenuController.hpp"
int __stdcall UpdateMenuPage0D(MenuControllerView *self);
extern "C" char *__stdcall MenuMusicLineProbe(
    char *cursor, int *remaining, char *destination, MenuControllerView *page, int callPage)
{
    if (callPage) UpdateMenuPage0D(page);
    char *next = CopyMenuMusicLine(cursor, remaining, destination);
    return SkipMenuMusicLine(next, remaining);
}
