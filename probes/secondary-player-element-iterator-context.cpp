typedef void *(__fastcall *SecondaryElementConstructor)(void *element);
void __stdcall ConstructSecondaryPlayerElementArray(
    void *array, int stride, int count, SecondaryElementConstructor construct);
// A real indirect callback prevents the compiler from deleting the loop when
// the callback is a separately known identity-only constructor.
extern "C" void __stdcall SecondaryPlayerElementIteratorProbe(
    void *array, int stride, int count, SecondaryElementConstructor construct)
{
    ConstructSecondaryPlayerElementArray(array, stride, count, construct);
}
