#include "ConfigurationAudioMenu.hpp"
#include "SecondaryDamage.hpp"
#include "MeshEffects.hpp"

// Compile-only independent inputs prevent the menu's fixed event, period and
// sound arguments from specializing the shared interfaces during /GL linking.
extern "C" __declspec(dllexport) int __stdcall ConfigurationMenuSupportProbe(int kind,
    SecondaryDebugSelectionView *selection, int amount, AnimationVmId id,
    short event, SecondaryDamageTimerView *timer, int period)
{
    switch (kind)
    {
    case 0: return MoveSecondaryDebugSelection(selection, amount);
    case 1: PushConfigurationSelection(selection); break;
    case 2: PopConfigurationSelection(selection); break;
    case 3: SignalAndExecuteConfigurationAnimation(id, event); break;
    case 4: return timer->Elapsed(period);
    case 5: QueueMeshEffectSound(amount); break;
    }
    return 0;
}
