struct InputBindingDefaultsView { InputBindingDefaultsView *InitializeInputBindingDefaults(); };
extern "C" InputBindingDefaultsView *__stdcall InputBindingDefaultsProbe(InputBindingDefaultsView *self)
{
    return self->InitializeInputBindingDefaults();
}
