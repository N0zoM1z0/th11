// LZSS binary-tree maintenance for the replay compressor. Every node access
// indexes the global table directly; a local table pointer changes VC9's
// addressing and register choice.
struct ReplayCompressionNode
{
    int parent, left, right;
};
extern ReplayCompressionNode g_ReplayCompressionNodes[8193];

void __stdcall SpliceReplayCompressionNode(int node, int replacement);
void __stdcall ReplaceReplayCompressionNode(int oldNode, int newNode);
int __stdcall FindReplayCompressionPredecessor(int node);

// 00442AB0. Remove a node; with two children the rightmost node of the left
// subtree takes its place.
void __fastcall DeleteReplayCompressionNode(int node)
{
    if (!g_ReplayCompressionNodes[node].parent) return;
    if (!g_ReplayCompressionNodes[node].right)
        SpliceReplayCompressionNode(node, g_ReplayCompressionNodes[node].left);
    else if (!g_ReplayCompressionNodes[node].left)
        SpliceReplayCompressionNode(node, g_ReplayCompressionNodes[node].right);
    else
    {
        int replacement = FindReplayCompressionPredecessor(node);
        DeleteReplayCompressionNode(replacement);
        ReplaceReplayCompressionNode(node, replacement);
    }
}

// 00442B00. Replace a node by its only child (or the empty sentinel).
void __stdcall SpliceReplayCompressionNode(int node, int replacement)
{
    g_ReplayCompressionNodes[replacement].parent = g_ReplayCompressionNodes[node].parent;
    if (g_ReplayCompressionNodes[g_ReplayCompressionNodes[node].parent].right == node)
        g_ReplayCompressionNodes[g_ReplayCompressionNodes[node].parent].right = replacement;
    else
        g_ReplayCompressionNodes[g_ReplayCompressionNodes[node].parent].left = replacement;
    g_ReplayCompressionNodes[node].parent = 0;
}

// 00442B50. Move a node's parent link and both children to newNode.
void __stdcall ReplaceReplayCompressionNode(int oldNode, int newNode)
{
    int parent = g_ReplayCompressionNodes[oldNode].parent;
    if (g_ReplayCompressionNodes[parent].left == oldNode)
        g_ReplayCompressionNodes[parent].left = newNode;
    else
        g_ReplayCompressionNodes[parent].right = newNode;
    g_ReplayCompressionNodes[newNode] = g_ReplayCompressionNodes[oldNode];
    g_ReplayCompressionNodes[g_ReplayCompressionNodes[newNode].left].parent = newNode;
    g_ReplayCompressionNodes[g_ReplayCompressionNodes[newNode].right].parent = newNode;
    g_ReplayCompressionNodes[oldNode].parent = 0;
}

// 00442BE0. Rightmost node of the left subtree.
int __stdcall FindReplayCompressionPredecessor(int node)
{
    int cursor = g_ReplayCompressionNodes[node].left;
    while (g_ReplayCompressionNodes[cursor].right)
        cursor = g_ReplayCompressionNodes[cursor].right;
    return cursor;
}
