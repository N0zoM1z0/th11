// Complete native audio-adjacent helper functions. Source-level ownership
// and original library/template identity remain unresolved.
struct AudioAdjacentNode
{
    int key;
    AudioAdjacentNode *next;
    unsigned int payload, resetA, resetB;
};

AudioAdjacentNode *__fastcall FindAudioAdjacentListNode(AudioAdjacentNode *head,
    int key)
{
    while (head && head->key != key) head = head->next;
    return head;
}

extern float g_AudioAdjacentScratchMatrix[16];

float *__stdcall SetAudioAdjacentScratchMatrix(
    float m0, float m1, float m2, float m3,
    float m4, float m5, float m6, float m7,
    float m8, float m9, float m10, float m11,
    float m12, float m13, float m14, float m15)
{
    g_AudioAdjacentScratchMatrix[0] = m0;
    g_AudioAdjacentScratchMatrix[1] = m1;
    g_AudioAdjacentScratchMatrix[2] = m2;
    g_AudioAdjacentScratchMatrix[3] = m3;
    g_AudioAdjacentScratchMatrix[4] = m4;
    g_AudioAdjacentScratchMatrix[5] = m5;
    g_AudioAdjacentScratchMatrix[6] = m6;
    g_AudioAdjacentScratchMatrix[7] = m7;
    g_AudioAdjacentScratchMatrix[8] = m8;
    g_AudioAdjacentScratchMatrix[9] = m9;
    g_AudioAdjacentScratchMatrix[10] = m10;
    g_AudioAdjacentScratchMatrix[11] = m11;
    g_AudioAdjacentScratchMatrix[12] = m12;
    g_AudioAdjacentScratchMatrix[13] = m13;
    g_AudioAdjacentScratchMatrix[14] = m14;
    g_AudioAdjacentScratchMatrix[15] = m15;
    return g_AudioAdjacentScratchMatrix;
}
