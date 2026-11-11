struct ReplayCompressionNode
{
    int parent, left, right;
};
extern ReplayCompressionNode g_ReplayCompressionNodes[8193];

// 00442B00: replace a node by an existing left/right child. The native
// routine does not check the zero replacement because the sentinel node's
// parent word is used on the empty side.
void __stdcall SpliceReplayCompressionNode(int node, int replacement)
{
    ReplayCompressionNode *tree = g_ReplayCompressionNodes;
    tree[replacement].parent = tree[node].parent;
    int parent = tree[node].parent;
    if (tree[parent].right == node)
        tree[parent].right = replacement;
    else
        tree[parent].left = replacement;
    tree[node].parent = 0;
}

// 00442BE0: rightmost successor from the left subtree.
int __stdcall FindReplayCompressionPredecessor(int node)
{
    int current = g_ReplayCompressionNodes[node].left;
    for (int right = g_ReplayCompressionNodes[current].right; right;
         right = g_ReplayCompressionNodes[current].right)
        current = right;
    return current;
}

// 00442B50: transfer both children and the parent link to replacement.
// The native algorithm intentionally writes the shared zero sentinel on
// absent branches, rather than guarding each parent assignment.
void __stdcall ReplaceReplayCompressionNode(int oldNode, int newNode)
{
    ReplayCompressionNode *tree = g_ReplayCompressionNodes;
    int parent = tree[oldNode].parent;
    if (tree[parent].left == oldNode)
        tree[parent].left = newNode;
    else
        tree[parent].right = newNode;
    tree[newNode].parent = tree[oldNode].parent;
    tree[newNode].left = tree[oldNode].left;
    tree[newNode].right = tree[oldNode].right;
    tree[tree[newNode].left].parent = newNode;
    tree[tree[newNode].right].parent = newNode;
    tree[oldNode].parent = 0;
}

// 00442AB0: recursively remove a tree node. When it has two children,
// extract the rightmost node in the left subtree and replace the removed one.
void __stdcall DeleteReplayCompressionNode(int node)
{
    ReplayCompressionNode *tree = g_ReplayCompressionNodes;
    if (!tree[node].parent) return;

    if (!tree[node].right)
    {
        SpliceReplayCompressionNode(node, tree[node].left);
        return;
    }
    if (!tree[node].left)
    {
        SpliceReplayCompressionNode(node, tree[node].right);
        return;
    }
    int replacement = FindReplayCompressionPredecessor(node);
    DeleteReplayCompressionNode(replacement);
    ReplaceReplayCompressionNode(node, replacement);
}
