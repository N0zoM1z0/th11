#include <stdlib.h>

// 004423A0. Okumura-style LZSS: 8-KiB window, 18-byte lookahead, one flag
// bit then eight literal bits or 13 position and four length bits.
struct ReplayCompressionNode { int parent, left, right; };
extern ReplayCompressionNode g_ReplayCompressionNodes[8193];
extern unsigned char g_ReplayCompressionWindow[8192];
extern int g_ReplayCompressionRoot;

void __cdecl ResetReplayCompressionTree();
int __stdcall FindReplayCompressionMatch(int position, int *matchPosition);
void __fastcall DeleteReplayCompressionNode(int node);
void __stdcall ReplaceReplayCompressionNode(int oldNode, int newNode);

unsigned char *__stdcall CompressReplayData(const void *data, int size, int *compressedSize)
{
    const unsigned char *input = static_cast<const unsigned char *>(data);
    unsigned char mask = 0x80;
    int bits = 0;
    unsigned char *output = static_cast<unsigned char *>(malloc(size * 2));
    if (!output) return 0;
    unsigned char *out = output;
    *compressedSize = 0;
    ResetReplayCompressionTree();

    const unsigned char *cursor = input;
    int position = 1;
    int count;
    int c;
    for (count = 0; count < 18; ++count)
    {
        c = cursor - input < size ? *cursor++ : -1;
        if (c == -1) break;
        g_ReplayCompressionWindow[position + count] = static_cast<unsigned char>(c);
    }
    int length = count;
    int matchLength = 0;
    int matchPosition = 0;
    g_ReplayCompressionRoot = 1;
    g_ReplayCompressionNodes[1].parent = 0x2000;
    g_ReplayCompressionNodes[1].right = 0;
    g_ReplayCompressionNodes[1].left = 0;

    while (length > 0)
    {
        if (matchLength > length) matchLength = length;
        if (matchLength <= 2)
        {
            matchLength = 1;
            bits |= mask;
            if (!(mask >>= 1)) { *out++ = static_cast<unsigned char>(bits); bits = 0; mask = 0x80; }
            for (unsigned int bit = 0x80; bit; bit >>= 1)
            {
                if (g_ReplayCompressionWindow[position] & bit) bits |= mask;
                if (!(mask >>= 1)) { *out++ = static_cast<unsigned char>(bits); bits = 0; mask = 0x80; }
            }
        }
        else
        {
            if (!(mask >>= 1)) { *out++ = static_cast<unsigned char>(bits); bits = 0; mask = 0x80; }
            for (unsigned int bit = 0x1000; bit; bit >>= 1)
            {
                if (matchPosition & bit) bits |= mask;
                if (!(mask >>= 1)) { *out++ = static_cast<unsigned char>(bits); bits = 0; mask = 0x80; }
            }
            for (unsigned int bit = 8; bit; bit >>= 1)
            {
                if ((matchLength - 3) & bit) bits |= mask;
                if (!(mask >>= 1)) { *out++ = static_cast<unsigned char>(bits); bits = 0; mask = 0x80; }
            }
        }
        for (int i = matchLength; i > 0; --i)
        {
            int end = (position + 18) & 0x1fff;
                    if (g_ReplayCompressionNodes[end].parent)
            {
                int replacement;
                if (!g_ReplayCompressionNodes[end].right)
                    replacement = g_ReplayCompressionNodes[end].left;
                else if (!g_ReplayCompressionNodes[end].left)
                    replacement = g_ReplayCompressionNodes[end].right;
                else
                {
                    replacement = g_ReplayCompressionNodes[end].left;
                    while (g_ReplayCompressionNodes[replacement].right)
                        replacement = g_ReplayCompressionNodes[replacement].right;
                    DeleteReplayCompressionNode(replacement);
                    ReplaceReplayCompressionNode(end, replacement);
                    goto removed;
                }
                g_ReplayCompressionNodes[replacement].parent = g_ReplayCompressionNodes[end].parent;
                if (g_ReplayCompressionNodes[g_ReplayCompressionNodes[end].parent].right == end)
                    g_ReplayCompressionNodes[g_ReplayCompressionNodes[end].parent].right = replacement;
                else
                    g_ReplayCompressionNodes[g_ReplayCompressionNodes[end].parent].left = replacement;
                g_ReplayCompressionNodes[end].parent = 0;
            }
removed:
            c = cursor - input < size ? *cursor++ : -1;
            if (c == -1)
                --length;
            else
                g_ReplayCompressionWindow[end] = static_cast<unsigned char>(c);
            position = (position + 1) & 0x1fff;
            if (length)
                matchLength = FindReplayCompressionMatch(position, &matchPosition);
        }
    }
    if (!(mask >>= 1)) { *out++ = static_cast<unsigned char>(bits); bits = 0; mask = 0x80; }
    for (unsigned int bit = 0x1000; bit; bit >>= 1)
        if (!(mask >>= 1)) { *out++ = static_cast<unsigned char>(bits); bits = 0; mask = 0x80; }
    *compressedSize = static_cast<int>(out - output);
    return output;
}
