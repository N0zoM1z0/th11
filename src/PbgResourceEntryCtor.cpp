#include "PbgResourceEntry.hpp"

// Native full 0x441C70..0x441C78: reset only the name pointer, preserving
// the untouched three metadata words for the directory reader to fill.
PbgResourceEntryView::PbgResourceEntryView() : name(0)
{
}
