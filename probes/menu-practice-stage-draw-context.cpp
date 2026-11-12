struct MenuControllerView;
extern int __stdcall DrawMenuPage08(MenuControllerView *self);
extern "C" int __stdcall MenuPracticeStageDrawProbe(MenuControllerView *self)
{
    return DrawMenuPage08(self);
}
