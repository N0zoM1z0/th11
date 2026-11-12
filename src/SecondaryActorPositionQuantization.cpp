// Complete Japanese TH11 1.00a 0x410580..0x410604. The engine's original
// private interface takes an XY float position in EDI and updates the
// shared mesh actor pointer at 0x4A8EB4. Expose the position normally in
// C++ rather than pretending the original calling convention is portable.
struct MeshEffectActorView;
extern MeshEffectActorView *g_MeshEffectActor;

namespace
{
template<typename T> __forceinline T &QuantizedField(unsigned char *owner, unsigned int offset)
{ return *reinterpret_cast<T *>(owner + offset); }
}

void __stdcall QuantizeSecondaryActorPosition(const float *position)
{
    unsigned char *owner = reinterpret_cast<unsigned char *>(g_MeshEffectActor);
    QuantizedField<int>(owner, 0x888) =
        static_cast<int>(static_cast<double>(position[0]) * 128.0);
    QuantizedField<int>(owner, 0x88c) =
        static_cast<int>(static_cast<double>(position[1]) * 128.0);
    float *normalized = reinterpret_cast<float *>(owner + 0x87c);
    normalized[0] = static_cast<float>(static_cast<double>(QuantizedField<int>(owner, 0x888)) * 0.0078125);
    normalized[1] = static_cast<float>(static_cast<double>(QuantizedField<int>(owner, 0x88c)) * 0.0078125);
    QuantizedField<int>(owner, 0x7644) = 1;
    QuantizedField<int>(owner, 0x7728) = 1;
    QuantizedField<int>(owner, 0x780c) = 1;
    QuantizedField<int>(owner, 0x78f0) = 1;
    QuantizedField<int>(owner, 0x79d4) = 1;
    QuantizedField<int>(owner, 0x7ab8) = 1;
    QuantizedField<int>(owner, 0x7b9c) = 1;
    QuantizedField<int>(owner, 0x7c80) = 1;
}
