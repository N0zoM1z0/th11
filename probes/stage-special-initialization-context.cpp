// Dynamic, source-only VC9 LTCG caller. This is not a game startup
// implementation and does not encode an original instruction sequence.
extern int __stdcall ActivateSecondaryPlayerSpecial();

extern "C" int __stdcall StageSpecialInitializationProbe()
{
    return ActivateSecondaryPlayerSpecial();
}
