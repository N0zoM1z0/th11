#pragma once
#include "SecondaryGameplay.hpp"
#include "AnimationVmFactory.hpp"
#include "FormattedTextureText.hpp"
#include "AnimationSpriteBinding.hpp"

struct SecondaryDialogueInstruction
{
    unsigned short frame;
    unsigned char opcode, payloadBytes;
};

// Observed dialogue fields, rather than an original class or allocation size.
struct SecondaryDialogueView
{
    int mode;
    unsigned char unknown004[0x18 - 4];
    StageTimerView time, wait;
    unsigned int player, opponent, panel, text0, text1, annotation0, annotation1, boss;
    unsigned int unknown060;
    SecondaryDialogueInstruction *instruction;
    CameraVector positions[3];
    int delay;
    unsigned int flags;
    int line, initialized, speaker;
    unsigned int colors[3];
};

struct SecondaryDialogueSessionView
{
    unsigned char unknown000[0x74];
    int mode;
};
extern SecondaryDialogueSessionView *g_SecondaryDialogueSession;
extern unsigned char g_SecondaryDialogueDecodedText[];

int __stdcall RunSecondaryDialogue(SecondaryDialogueView *self);
char *__stdcall DecodeSecondaryDialogueText(const unsigned char *text);
AnimationVmId __stdcall FindSecondaryDialogueChild(unsigned int *parent, int script);
void __stdcall BindSecondaryDialogueSprite(const AnimationVmId &id,
    AnimationDocumentView *file, int sprite);
void __stdcall ChangeSecondaryDialogueSprite(const AnimationVmId &id, int sprite);

void __cdecl PrepareSecondaryDialogueStageClear();
void __cdecl MarkSecondaryDialoguePlayerClear();
void __cdecl EnterSecondaryStageClearMenu();
void __cdecl AdvanceSecondaryDialogueStage();
int __stdcall SecondaryDialogueClearValue(SecondaryHostSelectionState *state);
int __stdcall FadeSecondaryDialogueAudio(float duration);

typedef char SecondaryDialogueInstructionSize[(sizeof(SecondaryDialogueInstruction) == 4) ? 1 : -1];
typedef char SecondaryDialogueTimeOffset[(offsetof(SecondaryDialogueView, time) == 0x18) ? 1 : -1];
typedef char SecondaryDialogueWaitOffset[(offsetof(SecondaryDialogueView, wait) == 0x2c) ? 1 : -1];
typedef char SecondaryDialogueTextOffset[(offsetof(SecondaryDialogueView, text0) == 0x4c) ? 1 : -1];
typedef char SecondaryDialogueInstructionOffset[(offsetof(SecondaryDialogueView, instruction) == 0x64) ? 1 : -1];
typedef char SecondaryDialoguePositionsOffset[(offsetof(SecondaryDialogueView, positions) == 0x68) ? 1 : -1];
typedef char SecondaryDialogueFlagsOffset[(offsetof(SecondaryDialogueView, flags) == 0x90) ? 1 : -1];
typedef char SecondaryDialogueColorsOffset[(offsetof(SecondaryDialogueView, colors) == 0xa0) ? 1 : -1];
