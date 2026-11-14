// TH11's 8-KiB LZSS dictionary is paired with an 8193-entry binary search tree.
struct ReplayCompressionNode
{
    int parent, left, right;
};
extern ReplayCompressionNode g_ReplayCompressionNodes[8193];
extern unsigned char g_ReplayCompressionWindow[8192];
extern int g_ReplayCompressionRoot;

void __stdcall ReplaceReplayCompressionNode(int oldNode, int newNode);

// 00442910. Insert a window position into the tree and return the longest
// match; an 18-byte match replaces the old node. VC9 unrolls the compare.
int __stdcall FindReplayCompressionMatch(int position, int *matchPosition)
{
    if (!position) return 0;
    int bestLength = 0;
    int candidate = g_ReplayCompressionRoot;
    for (;;)
    {
        int difference = 0;
        int length;
        for (length = 0; length < 18; ++length)
        {
            difference = g_ReplayCompressionWindow[(position + length) & 0x1fff] -
                g_ReplayCompressionWindow[(candidate + length) & 0x1fff];
            if (difference) break;
        }
        if (length >= bestLength)
        {
            bestLength = length;
            *matchPosition = candidate;
            if (length >= 18)
            {
                ReplaceReplayCompressionNode(candidate, position);
                return length;
            }
        }
        int *link = difference >= 0 ? &g_ReplayCompressionNodes[candidate].right
                                    : &g_ReplayCompressionNodes[candidate].left;
        if (!*link)
        {
            *link = position;
            g_ReplayCompressionNodes[position].parent = candidate;
            g_ReplayCompressionNodes[position].right = 0;
            g_ReplayCompressionNodes[position].left = 0;
            return bestLength;
        }
        candidate = *link;
    }
}
