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

// 00442BE0: native private EAX receiver/result, plain RET. Keep the three
// node-table references as DIR32 fields so the canonical replay binds them.
__declspec(naked) int __stdcall FindReplayCompressionPredecessor(int node)
{
    __asm {
        lea eax, [eax + eax*2]
        mov eax, dword ptr [eax*4 + g_ReplayCompressionNodes + 4]
        lea ecx, [eax + eax*2]
        mov ecx, dword ptr [ecx*4 + g_ReplayCompressionNodes + 8]
        test ecx, ecx
        je predecessor_done
        jmp predecessor_walk
        _emit 08Dh
        _emit 09Bh
        _emit 00h
        _emit 00h
        _emit 00h
        _emit 00h
    predecessor_walk:
        mov eax, ecx
        lea edx, [eax + eax*2]
        mov ecx, dword ptr [edx*4 + g_ReplayCompressionNodes + 8]
        test ecx, ecx
        jne predecessor_walk
    predecessor_done:
        ret
    }
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

// 00442B00 private ABI: ESI is the old node and EDX its replacement.
extern "C" __declspec(naked) void __cdecl ReplayCompressionSpliceNodePrivate()
{
    __asm {
        lea ecx, [esi + esi*2]
        push edi
        mov edi, dword ptr [ecx*4 + 04A8ED8h]
        lea eax, [edx + edx*2]
        mov dword ptr [eax*4 + 04A8ED8h], edi
        mov eax, dword ptr [ecx*4 + 04A8ED8h]
        lea eax, [eax + eax*2]
        add eax, eax
        add eax, eax
        pop edi
        cmp dword ptr [eax + 04A8EE0h], esi
        jne splice_right
        mov dword ptr [eax + 04A8EE0h], edx
        mov dword ptr [ecx*4 + 04A8ED8h], 0
        ret
    splice_right:
        mov dword ptr [eax + 04A8EDCh], edx
        mov dword ptr [ecx*4 + 04A8ED8h], 0
        ret
    }
}

// 00442B50 consumes EAX/EDX with no stack arguments.
extern "C" void __cdecl ReplayCompressionReplaceNodePrivate();

// 00442AB0 private ABI: ECX is the node; the two-child case tail-jumps to
// the EAX/EDX replacement helper after recursively removing the predecessor.
__declspec(naked) void __fastcall DeleteReplayCompressionNode(int node)
{
    __asm {
        push esi
        mov esi, ecx
        lea eax, [esi + esi*2]
        add eax, eax
        add eax, eax
        cmp dword ptr [eax + 04A8ED8h], 0
        je delete_done
        mov edx, dword ptr [eax + 04A8EE0h]
        test edx, edx
        jne delete_two_children
        mov edx, dword ptr [eax + 04A8EDCh]
    delete_splice_call:
        call ReplayCompressionSpliceNodePrivate
    delete_done:
        pop esi
        ret
    delete_two_children:
        cmp dword ptr [eax + 04A8EDCh], 0
        je delete_splice_call
        push edi
        mov eax, esi
        call FindReplayCompressionPredecessor
        mov edi, eax
        mov ecx, edi
        call DeleteReplayCompressionNode
        mov edx, edi
        pop edi
        mov eax, esi
        pop esi
        jmp ReplayCompressionReplaceNodePrivate
    }
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
