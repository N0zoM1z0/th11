// Two un-inventoried TH11 code islands beside the game-over menu switch
// table at 0042F340 and 0042F350. Ghidra lacks function definitions and
// incoming references, so original owner/class and exact ABI are open.
struct PauseSelectionComparisonView
{
    int value;

    // Conventional C++ members describe an ECX object and a stack value;
    // private register allocation is a separate codegen question.
    int IsDifferent(int expected) const;
    int IsSmaller(int expected) const;
};

int PauseSelectionComparisonView::IsDifferent(int expected) const
{
    return value != expected;
}

int PauseSelectionComparisonView::IsSmaller(int expected) const
{
    return value < expected;
}
