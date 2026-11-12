#include "PbgResourceSession.hpp"

// Complete native 0x4415A0..0x4415AF. Four independent DWORD zero stores,
// preserving the original ECX-this/EAX-result and ordinary RET.
PbgResourceSessionView::PbgResourceSessionView()
    : entries(0), count(0), temporary(0), stream(0)
{
}
