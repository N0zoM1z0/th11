#include "ThreadInfLifecycle.hpp"

// Original 0x4593C0..0x4593D4: private EAX receiver and exactly four member
// zeros after the virtual table store. The final two words are not cleared.
ThreadInfView::ThreadInfView()
    : thread(0), unknown08(0), stopRequested(0), pending(0)
{
}

// The inline virtual destructor in the header also emits the 16-byte
// non-deleting owner at 0x459410 and natural 36-byte scalar deleting owner
// at 0x4593E0 when constructed and destroyed under the real /GL producer.
