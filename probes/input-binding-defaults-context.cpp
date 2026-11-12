struct InputBindingDefaultsView { void InitializeInputBindingDefaults(); };
extern "C" void __stdcall InputBindingDefaultsProbe(InputBindingDefaultsView *self)
{
    self->InitializeInputBindingDefaults();
}
