#pragma once

// The five words are a replay-header default block populated by the native
// replay-settings loader. Their individual meanings remain deliberately opaque.
struct ReplayHeaderStorageDefaultsView
{
    unsigned int word000;
    unsigned int word004;
    unsigned int word008;
    unsigned int word00c;
    unsigned short word010;
};
extern ReplayHeaderStorageDefaultsView g_ReplayHeaderStorageDefaults;

// Private no-stack-argument initializer: ESI points at the embedded 0x3C view.
void __stdcall InitializeReplayHeaderStorage();
