struct SecondaryStateFieldsView;
int __stdcall ReadSecondaryState000A(const SecondaryStateFieldsView *);
void __stdcall WriteSecondaryState000A(int, SecondaryStateFieldsView *);
int __stdcall ReadSecondaryState000B(const SecondaryStateFieldsView *);
void __stdcall WriteSecondaryState000B(int, SecondaryStateFieldsView *);
int __stdcall ReadSecondaryState004(const SecondaryStateFieldsView *);
void __stdcall WriteSecondaryState004(int, SecondaryStateFieldsView *);
int __stdcall ReadSecondaryState008(const SecondaryStateFieldsView *);
void __stdcall WriteSecondaryState008(int, SecondaryStateFieldsView *);
void __stdcall WriteSecondaryState010(int, SecondaryStateFieldsView *);
void __stdcall StoreSecondaryStateValue(int *, int);
extern "C" int __stdcall SecondaryStateAccessorProbe(SecondaryStateFieldsView *state, int *location, int value)
{
    int total = ReadSecondaryState000A(state) + ReadSecondaryState000B(state) +
                ReadSecondaryState004(state) + ReadSecondaryState008(state);
    WriteSecondaryState000A(value, state);
    WriteSecondaryState000B(total, state);
    WriteSecondaryState004(value, state);
    WriteSecondaryState008(total, state);
    WriteSecondaryState010(value, state);
    StoreSecondaryStateValue(location, total);
    return total;
}
