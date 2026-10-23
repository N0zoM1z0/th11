#include "SurfaceComposite.hpp"

// Non-runnable LTCG context. All dimensions and both objects remain unknown.
extern "C" bool __stdcall SurfaceCompositeProbe(SurfaceCompositeView *buffer,
    IDirect3DSurface9 *surface, int x, int y, int width, int height)
{
    return buffer->BlendToSurface(surface, x, y, width, height);
}
