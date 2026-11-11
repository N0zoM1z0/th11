#include "SecondaryPlayerElementIterator.hpp"
#include <stddef.h>

// The native helper at 004021A0 is invoked by the 272-byte player storage
// constructor for each seven-element, eight-byte-stride array. The return
// value of each callback (a C++ this-pointer) is ignored by the iterator.
// The exact original nominal compiler-helper name is not established.
void __stdcall ConstructSecondaryPlayerElementArray(
    void *array, int stride, int count, SecondaryElementConstructor construct)
{
    unsigned char *item = static_cast<unsigned char *>(array);
    for (int remaining = count - 1; remaining >= 0; --remaining)
    {
        construct(item);
        item += stride;
    }
}
