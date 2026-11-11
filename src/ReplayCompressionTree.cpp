#include <string.h>
struct ReplayCompressionNode { int parent, left, right; };
extern ReplayCompressionNode g_ReplayCompressionNodes[8193];
extern unsigned char g_ReplayCompressionWindow[8192];
// Original 004428D0..00442903: reset the 8-KiB window and 8193 tree nodes.
// Pointer order is the original signed 32-bit address comparison.
void __cdecl ResetReplayCompressionTree()
{
    memset(g_ReplayCompressionWindow, 0, sizeof(g_ReplayCompressionWindow));
    int *field = &g_ReplayCompressionNodes[0].left;
    do
    {
        field[-1] = 0;
        field[0] = 0;
        field[1] = 0;
        field += 3;
    } while (reinterpret_cast<int>(field) < reinterpret_cast<int>(g_ReplayCompressionWindow));
}
