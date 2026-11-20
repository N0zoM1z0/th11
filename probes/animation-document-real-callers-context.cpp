#include "AnimationDocument.hpp"
#include "AsciiLifecycle.hpp"
#include "SecondaryAuxiliaryOwner.hpp"
#include "SecondaryDebugHudLifetime.hpp"
#include "SecondaryPlayerMovement.hpp"
#include "SecondaryProgramExtension.hpp"
#include "TitleMenuLifecycle.hpp"
#include "StageResource.hpp"

// This root retains the existing game callers and supplies a natural dynamic
// AcquireAnimationDocument call so LTCG can infer its internal register ABI.
extern int __stdcall InitializeSecondaryDebugHudVisuals(SecondaryDebugHudOwned *);
extern int __stdcall InitializeSecondaryPlayerController(SecondaryDamageReferencesView *);

extern "C" int __stdcall AnimationDocumentRealCallersProbe(
    AsciiManagerObject *ascii,
    SecondaryAuxiliaryOwnerView *auxiliary,
    SecondaryDebugHudOwned *hud,
    SecondaryDamageReferencesView *player,
    StageResourceView *stage,
    SecondaryProgramWithExtensions *program,
    const char *filename,
    void *extensionData,
    int animationIndex)
{
    int result = 0;
    result += AcquireAnimationDocument(animationIndex, filename) != 0;
    result += InitializeAsciiManager(ascii);
    result += InitializeSecondaryAuxiliaryOwner(auxiliary);
    result += RegisterSecondaryHudChains(hud);
    result += InitializeSecondaryDebugHudVisuals(hud);
    result += InitializeSecondaryPlayerController(player);
    result += InitializeTitleMenuResources();
    result += LoadStageResource(stage, filename);
    result += program->SecondaryProgramWithExtensions::ProcessExtension(extensionData);
    return result;
}
