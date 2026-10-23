// Non-runnable calls retaining unknown filenames and the actual log body.
#include "../src/Screenshot.hpp"

extern "C" int __stdcall ScreenshotProbe(const char *a, const char *b)
{
    int first = CaptureScreenshot(a);
    return first + CaptureScreenshot(b);
}
