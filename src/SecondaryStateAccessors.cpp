// Physical field offsets observed in ten independently bounded TH11
// secondary-state read/write leaves. These are conventional source
// interfaces; the target's private EAX/ECX argument allocation is recovered
// by a genuine optimized producer rather than by forced register operands.
struct SecondaryStateFieldsView
{
    int value000, value004, value008, unused00c, value010;
};

int __stdcall ReadSecondaryState000A(const SecondaryStateFieldsView *state)
{ return state->value000; }
void __stdcall WriteSecondaryState000A(int value, SecondaryStateFieldsView *state)
{ state->value000 = value; }
int __stdcall ReadSecondaryState000B(const SecondaryStateFieldsView *state)
{ return state->value000; }
void __stdcall WriteSecondaryState000B(int value, SecondaryStateFieldsView *state)
{ state->value000 = value; }
int __stdcall ReadSecondaryState004(const SecondaryStateFieldsView *state)
{ return state->value004; }
void __stdcall WriteSecondaryState004(int value, SecondaryStateFieldsView *state)
{ state->value004 = value; }
int __stdcall ReadSecondaryState008(const SecondaryStateFieldsView *state)
{ return state->value008; }
void __stdcall WriteSecondaryState008(int value, SecondaryStateFieldsView *state)
{ state->value008 = value; }
void __stdcall WriteSecondaryState010(int value, SecondaryStateFieldsView *state)
{ state->value010 = value; }
void __stdcall StoreSecondaryStateValue(int *location, int value)
{ *location = value; }
