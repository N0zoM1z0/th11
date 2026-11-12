extern void __stdcall ChangeSecondaryAnimationScript(unsigned int *id, int script);
extern "C" void __stdcall SecondaryAnimationScriptSwitchProbe(unsigned int *id, int script)
{
    ChangeSecondaryAnimationScript(id, script);
}
