#include "SecondarySpellStage.hpp"
#include "SecondaryActorRuntime.hpp"
#include "SecondaryPattern.hpp"
#include "MeshEffects.hpp"

// The original score-feedback renderer at 0x41F010 owns its separate
// jump dispatch and VM effects. We retain it as a real external callee
// rather than folding it into the stage lifecycle.
void __stdcall PublishSecondarySpellScoreEffect(int points);

struct SpellRecordStatusView;
extern SpellRecordStatusView *g_StageSpellRecordStatus; // 0x4A8EB8
extern unsigned char *g_StageSpellRecordData;           // 0x4A8EBC
extern int g_SecondaryActorScore;
extern int g_MeshEffectSelection0, g_MeshEffectSelection1;

namespace
{
__forceinline void IncrementCompletionRecord(unsigned char *record)
{
    int &successful = *reinterpret_cast<int *>(record + 0x80);
    if (successful < 99999)
        ++successful;
}
}

// Full original TH11 0x40CD60..0x40CEAE. The native tests stage flags
// on the existing singleton, signals all three follower VMs, publishes
// the score and counts the completed spell in two independent records.
void __stdcall FinishSecondarySpellStage()
{
    SecondarySpellStageView *stage =
        reinterpret_cast<SecondarySpellStageView *>(g_MeshEffectSpell);
    if (!(stage->flags & 1u))
        return;

    g_StagePrimary->flags2ff0 |= 1u;

    SignalSecondaryAnimation(&stage->followerIds[0]);
    SignalSecondaryAnimation(&stage->followerIds[1]);
    SignalSecondaryAnimation(&stage->followerIds[2]);

    // Native clears both active and pending effect bits before the release.
    stage->flags &= ~0x21u;
    ReleaseSecondaryAnimation(AnimationVmId(stage->positionVmId));
    stage->positionVmId = 0;

    if (stage->flags & 2u)
    {
        const int bonus = stage->score / 10;
        g_SecondaryActorScore = static_cast<int>(
            static_cast<unsigned int>(g_SecondaryActorScore) +
            static_cast<unsigned int>(bonus));
        if (g_SecondaryActorScore >= 1000000000)
            g_SecondaryActorScore = 999999999;
        PublishSecondarySpellScoreEffect(stage->score);

        unsigned char *recordData = g_StageSpellRecordData;
        const unsigned char *status =
            reinterpret_cast<const unsigned char *>(g_StageSpellRecordStatus);
        if (*reinterpret_cast<const int *>(status + 0x10) != 1)
        {
            const unsigned int spellOffset =
                static_cast<unsigned int>(stage->spellIndex) * 0x90u;
            const int characterMode =
                g_MeshEffectSelection0 * 3 + g_MeshEffectSelection1;
            unsigned char *first = recordData +
                static_cast<unsigned int>(characterMode) * 0x68d4u +
                spellOffset + 0x66c;
            IncrementCompletionRecord(first);

            unsigned char *second = recordData + spellOffset + 0x27b64;
            IncrementCompletionRecord(second);
        }
        QueueMeshEffectSound(0x2d);
    }
    else
    {
        PublishSecondarySpellScoreEffect(0);
    }
}
