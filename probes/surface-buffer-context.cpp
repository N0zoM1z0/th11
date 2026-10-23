// Non-runnable member contexts; the owner remains live after each operation.
#include "../src/SurfaceBuffer.hpp"

extern "C" int __stdcall SurfaceBufferProbe(SurfaceBufferView *buffer, IDirect3DSurface9 *surface)
{
    bool read = buffer->ReadFromSurface(surface);
    bool written = buffer->WriteToSurface(surface);
    return read + written + buffer->height;
}
