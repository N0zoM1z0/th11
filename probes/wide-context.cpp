// Non-runnable link context: drives both the secondary-gameplay owners and the
// full menu controller so shared callees see their real callers.
extern "C" int __stdcall SecondaryHudOwnersProbe(int, int, int);
extern "C" int __stdcall MenuAllProbe(int);
extern "C" int __stdcall WideProbe(int a, int b, int c)
{
    return SecondaryHudOwnersProbe(a, b, c) + MenuAllProbe(a);
}
