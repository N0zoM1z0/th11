#include <stdlib.h>
#include <string.h>

// TH11 004423A0..004426BA: 8-KiB sliding dictionary with an 18-byte
// lookahead and a binary-tree match index. The output packs one selector bit
// followed by either eight literal bits or thirteen position/four length bits.
struct ReplayCompressionNode { int parent, left, right; };
extern ReplayCompressionNode g_ReplayCompressionNodes[8193];
extern unsigned char g_ReplayCompressionWindow[8192];
extern int g_ReplayCompressionRoot;

void __cdecl ResetReplayCompressionTree();
int __stdcall FindReplayCompressionMatch(int position, int *matchPosition);
void __stdcall DeleteReplayCompressionNode(int position);
void __stdcall ReplaceReplayCompressionNode(int oldNode, int newNode);

namespace
{
struct ReplayCompressionBits
{
    unsigned char *out;
    unsigned char value;
    unsigned char mask;

    explicit ReplayCompressionBits(unsigned char *output)
        : out(output), value(0), mask(0x80) {}

    __forceinline void Put(bool set)
    {
        if (set) value |= mask;
        mask >>= 1;
        if (!mask)
        {
            *out++ = value;
            value = 0;
            mask = 0x80;
        }
    }
};
}

// 004423A0. The original retains the allocated output on success even if
// no input bytes are present. The caller owns that allocation.
unsigned char *__stdcall CompressReplayData(const void *data, int size,
                                            int *compressedSize)
{
    unsigned char *output = static_cast<unsigned char *>(malloc(size * 2));
    if (!output) return 0;

    *compressedSize = 0;
    ResetReplayCompressionTree();

    const unsigned char *read = static_cast<const unsigned char *>(data);
    int consumed = 0;
    int available = 0;
    while (consumed < size && available < 18)
    {
        g_ReplayCompressionWindow[available + 1] = *read++;
        ++consumed;
        ++available;
    }

    g_ReplayCompressionRoot = 1;
    g_ReplayCompressionNodes[1].parent = 8192;
    g_ReplayCompressionNodes[1].left = 0;
    g_ReplayCompressionNodes[1].right = 0;

    int position = 1;
    int matchedPosition = 0;
    int matchedLength = 0;
    ReplayCompressionBits bits(output);

    while (available > 0)
    {
        if (matchedLength > available) matchedLength = available;

        if (matchedLength <= 2)
        {
            matchedLength = 1;
            bits.Put(true);
            unsigned char literal = g_ReplayCompressionWindow[position];
            for (unsigned int bit = 0x80; bit; bit >>= 1)
                bits.Put((literal & bit) != 0);
        }
        else
        {
            bits.Put(false);
            for (unsigned int bit = 0x1000; bit; bit >>= 1)
                bits.Put((matchedPosition & bit) != 0);
            for (unsigned int bit = 8; bit; bit >>= 1)
                bits.Put(((matchedLength - 3) & bit) != 0);
        }

        const int advance = matchedLength;
        for (int i = 0; i < advance; ++i)
        {
            int end = (position + 18) & 0x1fff;
            // The original compiler inlines the single-child unlink here,
            // but retains calls for the two-child predecessor case.
            ReplayCompressionNode *tree = g_ReplayCompressionNodes;
            if (tree[end].parent)
            {
                int replacement = tree[end].right;
                if (!replacement)
                    replacement = tree[end].left;
                else if (tree[end].left)
                {
                    replacement = tree[end].left;
                    while (tree[replacement].right)
                        replacement = tree[replacement].right;
                    DeleteReplayCompressionNode(replacement);
                    ReplaceReplayCompressionNode(end, replacement);
                    goto removed;
                }

                tree[replacement].parent = tree[end].parent;
                int parent = tree[end].parent;
                if (tree[parent].right == end)
                    tree[parent].right = replacement;
                else
                    tree[parent].left = replacement;
                tree[end].parent = 0;
            }
removed:
            if (consumed < size)
            {
                g_ReplayCompressionWindow[end] = *read++;
                ++consumed;
            }
            else --available;

            position = (position + 1) & 0x1fff;
            if (available > 0)
                matchedLength = FindReplayCompressionMatch(position, &matchedPosition);
        }
    }

    // The target terminates with a 0 selector and thirteen zero bits.
    // It writes only completed bytes: no unconditional final-byte flush.
    bits.Put(false);
    for (unsigned int bit = 0x1000; bit; bit >>= 1)
        bits.Put(false);

    *compressedSize = static_cast<int>(bits.out - output);
    return output;
}
