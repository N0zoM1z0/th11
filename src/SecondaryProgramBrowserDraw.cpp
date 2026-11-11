#include "SecondaryProgramBrowser.hpp"
#include "AsciiFormatting.hpp"
#include "AsciiLifecycle.hpp"

namespace
{
__forceinline AsciiManagerView *TextManager()
{
    return reinterpret_cast<AsciiManagerView *>(g_AsciiManager);
}

__forceinline void ShowFile(SecondaryProgramBrowserView *browser,
    const float position[3], const char *format)
{
    AsciiManagerPrint(TextManager(), position, format,
        browser->filenames[browser->fileSelection.current]);
}
}

// Native TH11 00438E70..00439093: ECL program browser's debug text overlay.
// Reads the browser's physical ESI state and publishes each line to the
// shared ASCII manager. It does not draw sprites or own the script manager.
int __stdcall DrawSecondaryProgramBrowser(SecondaryProgramBrowserView *browser)
{
    // Print/append is a game callback that may update the singleton;
    // the native owner reloads it between individual formatted lines.
    float position[3] = {0.0f, 0.0f, 0.0f};
    AsciiManagerPrint(TextManager(), position, "Spt Test\n");

    switch (browser->state)
    {
    case 1:
        position[0] = 42.0f;
        position[1] = 16.0f;
        if (browser->fileCount)
            ShowFile(browser, position, "File %s");
        else
            AsciiManagerPrint(TextManager(), position, "File not found.");

        position[1] = 26.0f;
        if (browser->program)
            AsciiManagerPrint(TextManager(), position, "Ecl %s",
                browser->program->subroutines[browser->subroutineSelection.current].name);
        else
            AsciiManagerPrint(TextManager(), position, "Ecl not load");

        position[1] = 36.0f;
        AsciiManagerPrint(TextManager(), position, "Quit");
        position[0] = 32.0f;
        position[1] = static_cast<float>(
            static_cast<double>(browser->category.current) * 10.0 + 16.0);
        AsciiManagerPrint(TextManager(), position, ">");
        TextManager()->color = 0xffffffff;
        break;
    case 2:
        position[0] = 32.0f;
        position[1] = 16.0f;
        TextManager()->color = 0xffff4040;
        ShowFile(browser, position, "Loading %s");
        TextManager()->color = 0xffffffff;
        break;
    case 3:
        position[0] = 32.0f;
        position[1] = 16.0f;
        TextManager()->color = 0xffa0a0a0;
        AsciiManagerPrint(TextManager(), position, "Pos %.3d %.3d",
            static_cast<int>(browser->displayOrigin),
            static_cast<int>(browser->displayScale));
        TextManager()->color = 0xffffffff;
        break;
    }
    return 1;
}
