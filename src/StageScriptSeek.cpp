#include "StageScript.hpp"

// Scan the stage's instruction stream for opcode 16 with the requested marker.
// The marker is passed in the second fastcall register, as in the native caller.
// The stream is terminated by a negative frame; reaching that terminator is a
// no-op. The script cursor advances past the matching instruction, not onto it.
void __fastcall SeekStageScriptMarker(void *unused, int marker)
{
    (void)unused;
    StageResourceView *stage = g_StagePrimary;
    StageScriptInstruction *cursor = reinterpret_cast<StageScriptInstruction *>(stage->section008);
    while (cursor->frame >= 0)
    {
        if (cursor->opcode == 16 && cursor->args.words[0] == marker)
        {
            StageScriptInstruction *next = reinterpret_cast<StageScriptInstruction *>(
                reinterpret_cast<unsigned char *>(cursor) + cursor->byteSize);
            StageScriptStateView &state = *reinterpret_cast<StageScriptStateView *>(stage);
            state.instruction = next;
            StageTimerView &timer = state.scriptTime;
            int frame = state.instruction->frame;
            if (!(timer.flags & 1))
            {
                timer.fraction = 0.0f;
                timer.current = 0;
                timer.previous = -999999;
                timer.scale = &g_AnimationTimeScale;
                timer.flags |= 1;
            }
            timer.current = frame;
            timer.previous = static_cast<int>(static_cast<unsigned int>(frame) - 1);
            timer.fraction = static_cast<float>(frame);
            return;
        }
        cursor = reinterpret_cast<StageScriptInstruction *>(
            reinterpret_cast<unsigned char *>(cursor) + cursor->byteSize);
    }
}
