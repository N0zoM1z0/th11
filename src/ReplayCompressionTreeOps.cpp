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
    ReplayCompressionNode *tree = g_ReplayCompressionNodes;
    int cursor = tree[node].left;
    while (tree[cursor].right)
        cursor = tree[cursor].right;
    return cursor;
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
void __fastcall DeleteReplayCompressionNode(int node)
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


// Native 00442B50 private ABI: EAX is the node being replaced, EDX is its
// replacement. No stack arguments are consumed; the caller owns both values.
extern "C" __declspec(naked) void __cdecl ReplayCompressionReplaceNodePrivate()
{
    __asm {
        push esi
        mov esi, eax
        lea ecx, [esi + esi*2]
        mov eax, dword ptr [ecx*4 + 04A8ED8h]
        lea eax, [eax + eax*2]
        add eax, eax
        add eax, eax
        cmp dword ptr [eax + 04A8EDCh], esi
        jne replace_right
        mov dword ptr [eax + 04A8EDCh], edx
        jmp replace_children
    replace_right:
        mov dword ptr [eax + 04A8EE0h], edx
    replace_children:
        mov esi, dword ptr [ecx*4 + 04A8ED8h]
        lea eax, [edx + edx*2]
        add eax, eax
        mov dword ptr [eax + eax + 04A8ED8h], esi
        mov esi, dword ptr [ecx*4 + 04A8EDCh]
        add eax, eax
        mov dword ptr [eax + 04A8EDCh], esi
        mov esi, dword ptr [ecx*4 + 04A8EE0h]
        mov dword ptr [eax + 04A8EE0h], esi
        mov esi, dword ptr [eax + 04A8EDCh]
        lea esi, [esi + esi*2]
        mov dword ptr [esi*4 + 04A8ED8h], edx
        mov eax, dword ptr [eax + 04A8EE0h]
        lea eax, [eax + eax*2]
        mov dword ptr [eax*4 + 04A8ED8h], edx
        mov dword ptr [ecx*4 + 04A8ED8h], 0
        pop esi
        ret
    }
}
