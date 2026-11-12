struct MenuControllerView;
extern int __fastcall DrawMenuController(MenuControllerView *);
extern "C" int __stdcall MenuControllerDrawProbe(MenuControllerView *self)
{
    return DrawMenuController(self);
}
