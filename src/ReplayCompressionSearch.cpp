// TH11's 8-KiB LZSS dictionary is paired with a 8193-entry binary search tree.
// Tree node addresses are adjacent to the dictionary in the original image.
struct ReplayCompressionNode
{
    int parent, left, right;
};
extern ReplayCompressionNode g_ReplayCompressionNodes[8193];
extern unsigned char g_ReplayCompressionWindow[8192];
extern int g_ReplayCompressionRoot;

// 00442B50 is the existing-node replacement helper; the source ABI is
// conventional, not the target's private register allocation.
void __stdcall ReplaceReplayCompressionNode(int oldNode, int newNode);

// 00442910..00442AAF: find a match for the circular dictionary position,
// insert it into the tree, or replace the old node on an 18-byte match.
int __stdcall FindReplayCompressionMatch(int position, int *matchPosition)
{
    if (!position) return 0;

    int bestLength = 0;
    int candidate = g_ReplayCompressionRoot;
    for (;;)
    {
        int difference = 0;
        int length = 0;
        // The original compares up to 18 bytes in groups of six.
        do
        {
            difference = static_cast<int>(g_ReplayCompressionWindow[(position + length + 0) & 0x1fff]) -
                static_cast<int>(g_ReplayCompressionWindow[(candidate + length + 0) & 0x1fff]);
            if (difference) { length += 0; goto compared; }
            difference = static_cast<int>(g_ReplayCompressionWindow[(position + length + 1) & 0x1fff]) -
                static_cast<int>(g_ReplayCompressionWindow[(candidate + length + 1) & 0x1fff]);
            if (difference) { length += 1; goto compared; }
            difference = static_cast<int>(g_ReplayCompressionWindow[(position + length + 2) & 0x1fff]) -
                static_cast<int>(g_ReplayCompressionWindow[(candidate + length + 2) & 0x1fff]);
            if (difference) { length += 2; goto compared; }
            difference = static_cast<int>(g_ReplayCompressionWindow[(position + length + 3) & 0x1fff]) -
                static_cast<int>(g_ReplayCompressionWindow[(candidate + length + 3) & 0x1fff]);
            if (difference) { length += 3; goto compared; }
            difference = static_cast<int>(g_ReplayCompressionWindow[(position + length + 4) & 0x1fff]) -
                static_cast<int>(g_ReplayCompressionWindow[(candidate + length + 4) & 0x1fff]);
            if (difference) { length += 4; goto compared; }
            difference = static_cast<int>(g_ReplayCompressionWindow[(position + length + 5) & 0x1fff]) -
                static_cast<int>(g_ReplayCompressionWindow[(candidate + length + 5) & 0x1fff]);
            if (difference) { length += 5; goto compared; }
            length += 6;
        } while (length < 18);

compared:
        if (bestLength <= length)
        {
            *matchPosition = candidate;
            bestLength = length;
            if (length >= 18)
            {
                ReplaceReplayCompressionNode(candidate, position);
                return bestLength;
            }
        }

        int *link = difference < 0 ?
            &g_ReplayCompressionNodes[candidate].left :
            &g_ReplayCompressionNodes[candidate].right;
        if (*link)
        {
            candidate = *link;
            continue;
        }

        *link = position;
        g_ReplayCompressionNodes[position].parent = candidate;
        g_ReplayCompressionNodes[position].left = 0;
        g_ReplayCompressionNodes[position].right = 0;
        return bestLength;
    }
}
