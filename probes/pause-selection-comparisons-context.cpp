// Diagnostic only: calls both comparison members with generic values.
struct PauseSelectionComparisonView
{
    int value;
    int IsDifferent(int expected) const;
    int IsSmaller(int expected) const;
};

extern "C" int __stdcall PauseSelectionComparisonsProbe(const PauseSelectionComparisonView *self, int expected)
{
    return self->IsDifferent(expected) + self->IsSmaller(expected + 1);
}
