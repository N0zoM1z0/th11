struct MenuControllerView;
extern int __stdcall DrawMenuPage0A(MenuControllerView *self);
extern "C" int __stdcall MenuReplayRecordsDrawProbe(MenuControllerView *self)
{
    return DrawMenuPage0A(self);
}
