#include <new>

// Physical 0x18B0 replay segment. The member link offsets are independently
// confirmed by the complete original 93-byte clear-stage owner.
struct ReplayCleanupSegment
{
    unsigned char unknown000[0x18a8];
    ReplayCleanupSegment *previous;
    ReplayCleanupSegment *next;
};
struct ReplayCleanupNode
{
    ReplayCleanupSegment *segment;
    ReplayCleanupNode *next, *previous;
};
struct ReplayCleanupStream
{
    ReplayCleanupNode *first, *unknown, *last;
};
struct ReplayCleanupState
{
    unsigned char unknown000[0x40];
    ReplayCleanupStream streams[8];
};

// Native complete 0x437420..0x43747C, one 33-instruction function.
void __stdcall ClearReplayStageSegments(ReplayCleanupState *state, int stage)
{
    ReplayCleanupNode *node = state->streams[stage].first;
    while (node)
    {
        ReplayCleanupNode *next = node->next;
        ReplayCleanupSegment *segment = node->segment;
        if (segment)
        {
            if (segment->previous)
                reinterpret_cast<ReplayCleanupNode *>(segment->previous)->previous =
                    reinterpret_cast<ReplayCleanupNode *>(segment->next);
            if (segment->next)
                reinterpret_cast<ReplayCleanupNode *>(segment->next)->next =
                    reinterpret_cast<ReplayCleanupNode *>(segment->previous);
            segment->previous = 0;
            segment->next = 0;
            ::operator delete(segment);
        }
        node = next;
    }
}
