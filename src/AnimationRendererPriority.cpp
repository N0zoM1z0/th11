#include "AnimationRendererDispatch.hpp"

// Callback registration priorities are not array indices. The renderer
// selects a dense layer number for each callback, then forwards the shared
// draw routine's result without applying another status conversion.
#define DRAW_PRIORITY_CALLBACK(priority, layer) \
    int __fastcall AnimationRendererDrawPriority##priority(void *context) \
    { \
        return DrawAnimationRendererLayer(static_cast<AnimationRenderer *>(context), layer); \
    }

DRAW_PRIORITY_CALLBACK(04, 0)
DRAW_PRIORITY_CALLBACK(06, 1)
DRAW_PRIORITY_CALLBACK(08, 2)
DRAW_PRIORITY_CALLBACK(12, 4)
DRAW_PRIORITY_CALLBACK(15, 5)
DRAW_PRIORITY_CALLBACK(16, 6)
DRAW_PRIORITY_CALLBACK(17, 7)
DRAW_PRIORITY_CALLBACK(18, 8)
DRAW_PRIORITY_CALLBACK(19, 9)
DRAW_PRIORITY_CALLBACK(21, 10)
DRAW_PRIORITY_CALLBACK(23, 11)
DRAW_PRIORITY_CALLBACK(24, 12)
DRAW_PRIORITY_CALLBACK(26, 13)
DRAW_PRIORITY_CALLBACK(28, 14)
DRAW_PRIORITY_CALLBACK(32, 15)
DRAW_PRIORITY_CALLBACK(34, 16)
DRAW_PRIORITY_CALLBACK(36, 17)
DRAW_PRIORITY_CALLBACK(39, 18)
DRAW_PRIORITY_CALLBACK(61, 24)
DRAW_PRIORITY_CALLBACK(60, 23)
DRAW_PRIORITY_CALLBACK(50, 22)
DRAW_PRIORITY_CALLBACK(49, 21)

#undef DRAW_PRIORITY_CALLBACK
