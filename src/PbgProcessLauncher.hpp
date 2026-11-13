#pragma once
// Complete Pbg helper at 0x442E40: create a Win32 child with its native
// STARTUPINFOA defaults; optionally poll process exit status until complete.
// Original source TU attribution and ownership of returned handles are open.
int __stdcall LaunchPbgChildProcess(const char *application, char *commandLine, int waitForExit);
