// Exact identity-only thiscall constructors at 004021D0 and 00433F00.
// They are separate physical three-byte code owners, passed as callback
// addresses at 0042F7ED and 0042F801 to the native generic seven-element
// vector constructor. Types are plausible physical 8-byte element views;
// original C++ class names and TU ownership are not established.
struct PlayerEmitterPolarPair
{
    float x, y;
    PlayerEmitterPolarPair();
};
struct PlayerEmitterOffsetPair
{
    int x, y;
    PlayerEmitterOffsetPair();
};
// No field writes: native both bodies are MOV EAX, ECX; RET.
PlayerEmitterPolarPair::PlayerEmitterPolarPair() {}
PlayerEmitterOffsetPair::PlayerEmitterOffsetPair() {}
