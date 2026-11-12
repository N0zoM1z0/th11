// Physical input binding layout. The original nominal interface/TU is not
// known; these offsets are derived from all 98 bytes of the native initializer.
struct InputBindingDefaultsView
{
    unsigned char unused000[0x134];
    short bindings[9];
    InputBindingDefaultsView *InitializeInputBindingDefaults();
};

// TH11 Japanese v1.00a 0x459BD0..0x459C31: the nine original WORD defaults.
InputBindingDefaultsView *InputBindingDefaultsView::InitializeInputBindingDefaults()
{
    bindings[0] = 0;
    bindings[1] = 1;
    bindings[2] = 2;
    bindings[8] = 3;
    bindings[3] = 4;
    bindings[4] = -1;
    bindings[5] = -1;
    bindings[6] = -1;
    bindings[7] = -1;
    return this;
}
