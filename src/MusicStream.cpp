#include "MusicStream.hpp"
#include <stdlib.h>
#include <string.h>

// Actual one-RET trace entry at45B5A0, without an invented implementation.
void __cdecl TraceMusicStream(const char *format, ...);

namespace
{
const HRESULT NotInitialized = static_cast<HRESULT>(0x800401f0);
const HRESULT BufferLost = static_cast<HRESULT>(0x88780096);

__forceinline HRESULT RestoreBuffer(SoundBufferView *buffer, BOOL *restored)
{
    if (restored) *restored = FALSE;
    DWORD status;
    HRESULT result = buffer->GetStatus(&status);
    if (FAILED(result)) return result;
    if (status & 2)
    {
        do
        {
            result = buffer->Restore();
            if (result == BufferLost) Sleep(10);
            result = buffer->Restore();
        } while (result);
        if (restored) *restored = TRUE;
        return S_OK;
    }
    return S_FALSE;
}

__forceinline HRESULT ResetDecoder(MusicDecoderView *self, DWORD offset, bool loop)
{
    if (self->memoryMode)
    {
        self->cursor = self->memoryStart;
        if (static_cast<LONG>(self->record->dataSize) > 0)
            self->memorySize = self->record->dataSize;
        if (loop && self->record->loopStart > 0)
            self->cursor = self->memoryStart + self->record->loopStart;
        return S_OK;
    }
    if (!self->file) return NotInitialized;
    if (loop && self->record->loopStart > 0)
    {
        SetFilePointer(self->file, self->record->dataOffset +
            self->record->loopStart + g_MusicArchiveOffset, 0, FILE_BEGIN);
        self->remaining = self->record->dataSize - self->record->loopStart;
    }
    else if (!offset)
    {
        SetFilePointer(self->file, self->record->dataOffset + g_MusicArchiveOffset, 0, FILE_BEGIN);
        self->remaining = self->record->dataSize;
    }
    else
    {
        SetFilePointer(self->file, offset + g_MusicArchiveOffset, 0, FILE_BEGIN);
        self->remaining = self->record->dataSize + self->record->dataOffset - offset;
    }
    return S_OK;
}
__forceinline int SilenceByte(const MusicDecoderView *self)
{
    return self->record->bitsPerSample == 8 ? 128 : 0;
}
}

HRESULT __stdcall RestoreMusicBuffer(SoundBufferView *buffer, BOOL *restored)
{
    return RestoreBuffer(buffer, restored);
}

SoundBufferView *__stdcall SelectAvailableMusicBuffer(MusicControllerView *self)
{
    if (!self->buffers) return 0;
    unsigned int index;
    for (index = 0; index < self->bufferCount; ++index)
    {
        if (!self->buffers[index]) continue;
        DWORD status = 0;
        self->buffers[index]->GetStatus(&status);
        if (!(status & 1)) break;
    }
    if (index == self->bufferCount)
        return self->buffers[static_cast<unsigned int>(rand()) % self->bufferCount];
    return self->buffers[index];
}

HRESULT __stdcall RefillMusicBuffer(MusicControllerView *self, SoundBufferView *buffer, int loop)
{
    if (!buffer) return NotInitialized;
    HRESULT result = RestoreBuffer(buffer, 0);
    if (FAILED(result)) return result;
    void *data;
    DWORD bytes;
    result = buffer->Lock(0, self->bufferBytes, &data, &bytes, 0, 0, 0);
    if (FAILED(result)) return result;
    ResetDecoder(self->decoder, 0, false);
    DWORD read;
    result = ReadMusicDecoder(self->decoder, data, bytes, &read);
    if (FAILED(result)) return result;
    if (!read)
        memset(data, SilenceByte(self->decoder), bytes);
    else if (read < bytes)
    {
        if (loop)
        {
            DWORD filled = read;
            do
            {
                ResetDecoder(self->decoder, 0, false);
                result = ReadMusicDecoder(self->decoder,
                    static_cast<unsigned char *>(data) + filled, bytes - filled, &read);
                if (FAILED(result)) return result;
                filled += read;
            } while (filled < bytes);
        }
        else memset(static_cast<unsigned char *>(data) + read,
            SilenceByte(self->decoder), bytes - read);
    }
    buffer->Unlock(data, bytes, 0, 0);
    return S_OK;
}

HRESULT __stdcall ResetMusicBufferPositions(MusicControllerView *self)
{
    if (!self->buffers) return NotInitialized;
    HRESULT result = S_OK;
    for (unsigned int i = 0; i < self->bufferCount; ++i)
        result |= self->buffers[i]->SetCurrentPosition(0);
    return result;
}

HRESULT __stdcall PlayMusicStream(MusicControllerView *self)
{
    if (!self->buffers) return NotInitialized;
    SoundBufferView *buffer = SelectAvailableMusicBuffer(self);
    if (!buffer) return E_FAIL;
    BOOL restored;
    HRESULT result = RestoreMusicBuffer(buffer, &restored);
    if (FAILED(result)) return result;
    if (restored)
    {
        result = RefillMusicBuffer(self, buffer, 0);
        if (FAILED(result)) return result;
        ResetMusicBufferPositions(self);
    }
    self->fadeEnabled = self->fadeRemaining = self->fadeInitial = 0;
    SetMusicBufferVolume(self, 0);
    self->active = 1;
    self->playPriority = 0;
    self->playFlags = 1;
    self->unknown02c = 0;
    return buffer->Play(0, 0, 1);
}

HRESULT __stdcall StopMusicStream(MusicControllerView *self, int closeFile)
{
    if (!self->buffers) return NotInitialized;
    self->active = self->paused = 0;
    HRESULT result = S_OK;
    for (unsigned int i = 0; i < self->bufferCount; ++i)
    {
        result |= self->buffers[i]->Stop();
        result |= self->buffers[i]->SetCurrentPosition(0);
    }
    self->fadeEnabled = 0;
    if (closeFile && self->decoder->closeOnStop == 1)
    {
        CloseHandle(self->decoder->file);
        self->decoder->file = INVALID_HANDLE_VALUE;
    }
    return result;
}

HRESULT __stdcall PauseMusicStream(MusicControllerView *self)
{
    if (!self->buffers) return NotInitialized;
    if (!self->active)
    {
        self->paused = 0;
        return NotInitialized;
    }
    self->active = 0;
    self->paused = 1;
    HRESULT result = self->buffers[0]->Stop();
    MusicDecoderView *decoder = self->decoder;
    decoder->savedFilePosition = SetFilePointer(decoder->file, 0, 0, FILE_CURRENT);
    decoder = self->decoder;
    if (decoder->closeOnStop == 1)
    {
        CloseHandle(decoder->file);
        decoder->file = INVALID_HANDLE_VALUE;
    }
    return result;
}

HRESULT __stdcall ResumeMusicStream(MusicControllerView *self)
{
    if (!self->buffers || !self->paused) return NotInitialized;
    self->paused = 0;
    MusicDecoderView *decoder = self->decoder;
    ConfigureMusicDecoder(decoder, decoder->record, decoder->savedFilePosition);
    self->active = 1;
    return self->buffers[0]->Play(0, self->playPriority, self->playFlags);
}

HRESULT __stdcall UpdateMusicStream(MusicControllerView *self)
{
    if (!self->buffers || !self->decoder) return NotInitialized;
    DWORD play, write;
    self->buffers[0]->GetCurrentPosition(&play, &write);
    if (self->nextWriteOffset >= write - self->notificationBytes &&
        self->nextWriteOffset < write) return NotInitialized;
    SoundBufferView *buffer = self->buffers[0];
    if (!buffer) return NotInitialized;
    BOOL restored;
    HRESULT result = RestoreMusicBuffer(buffer, &restored);
    if (FAILED(result)) return result;
    if (restored)
    {
        result = RefillMusicBuffer(self, self->buffers[0], 0);
        return FAILED(result) ? result : S_OK;
    }
    void *first = 0, *second = 0;
    DWORD firstBytes, secondBytes;
    result = self->buffers[0]->Lock(self->nextWriteOffset, self->notificationBytes,
        &first, &firstBytes, &second, &secondBytes, 0);
    if (FAILED(result)) return result;
    if (second) return E_UNEXPECTED;
    if (!self->fillSilence)
    {
        DWORD read;
        result = ReadMusicDecoder(self->decoder, first, firstBytes, &read);
        if (FAILED(result)) return result;
        if (read < firstBytes)
        {
            DWORD filled = read;
            do
            {
                result = ResetMusicDecoder(self->decoder, 0, true);
                if (FAILED(result)) return result;
                result = ReadMusicDecoder(self->decoder,
                    static_cast<unsigned char *>(first) + filled, firstBytes - filled, &read);
                if (FAILED(result)) return result;
                filled += read;
            } while (filled < firstBytes);
        }
    }
    else memset(first, SilenceByte(self->decoder), firstBytes);
    self->buffers[0]->Unlock(first, firstBytes, 0, 0);
    DWORD position;
    result = self->buffers[0]->GetCurrentPosition(&position, 0);
    if (FAILED(result)) return result;
    DWORD previous = self->lastPlayPosition;
    self->playProgress += position < previous ? self->bufferBytes - previous + position : position - previous;
    self->lastPlayPosition = position;
    if (self->fillSilence && self->playProgress >= self->decoder->dataSize)
        self->buffers[0]->Stop();
    self->nextWriteOffset = (self->nextWriteOffset + firstBytes) % self->bufferBytes;
    return S_OK;
}

HRESULT __stdcall ResetMusicPosition(MusicControllerView *self)
{
    if (!self->buffers[0] || !self->decoder) return NotInitialized;
    self->lastPlayPosition = self->playProgress = self->nextWriteOffset = 0;
    self->fillSilence = 0;
    SoundBufferView *buffer = self->buffers[0];
    if (!buffer) return NotInitialized;
    BOOL restored;
    HRESULT result = RestoreMusicBuffer(buffer, &restored);
    if (FAILED(result)) return result;
    if (restored)
    {
        result = RefillMusicBuffer(self, self->buffers[0], 0);
        if (FAILED(result)) return result;
    }
    ResetDecoder(self->decoder, 0, false);
    return self->buffers[0]->SetCurrentPosition(0);
}

HRESULT __stdcall OpenMusicDecoderFile(MusicDecoderView *self, const char *filename, SoundTrackRecord *record)
{
    if (!filename) return E_INVALIDARG;
    TraceMusicStream(g_MusicOpenTrace, filename);
    self->file = CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, 0);
    if (self->file == INVALID_HANDLE_VALUE) return E_FAIL;
    self->record = record;
    self->filename = filename;
    ResetDecoder(self, 0, false);
    self->dataSize = self->remaining;
    return S_OK;
}

HRESULT __stdcall ConfigureMusicDecoder(MusicDecoderView *self, SoundTrackRecord *record, int offset)
{
    if (self->memoryMode) return E_FAIL;
    if (self->file == INVALID_HANDLE_VALUE)
    {
        self->closeOnStop = 1;
        self->memoryMode = 0;
        OpenMusicDecoderFile(self, self->filename, record);
        if (self->file == INVALID_HANDLE_VALUE) return E_FAIL;
    }
    self->record = record;
    TraceMusicStream(g_MusicConfigureTrace, record, record->loopStart, record->dataSize);
    ResetMusicDecoder(self, static_cast<DWORD>(offset), false);
    self->dataSize = self->remaining;
    return S_OK;
}

HRESULT __stdcall ResetMusicDecoder(MusicDecoderView *self, DWORD offset, bool loop)
{
    return ResetDecoder(self, offset, loop);
}

HRESULT __stdcall ReadMusicDecoder(MusicDecoderView *self, void *destination, DWORD bytes, DWORD *read)
{
    if (self->memoryMode)
    {
        const unsigned char *source = self->cursor;
        if (!source) return NotInitialized;
        if (read) *read = 0;
        ULONG_PTR cursor = reinterpret_cast<ULONG_PTR>(source);
        ULONG_PTR end = reinterpret_cast<ULONG_PTR>(self->memoryStart) + self->memorySize;
        if (cursor + bytes > end)
            bytes = static_cast<DWORD>(end - cursor);
        memcpy(destination, source, bytes);
        self->cursor += bytes;
        if (read) *read = bytes;
        return S_OK;
    }
    if (!self->file) return NotInitialized;
    if (!destination || !read) return E_INVALIDARG;
    DWORD count = bytes <= self->remaining ? bytes : self->remaining;
    self->remaining -= count;
    DWORD transferred;
    ReadFile(self->file, destination, count, &transferred, 0);
    *read = transferred;
    return S_OK;
}

HRESULT __stdcall QueryMusicBufferInterface(MusicControllerView *self, DWORD index, void **object)
{
    if (!self->buffers) return NotInitialized;
    if (index >= self->bufferCount) return E_INVALIDARG;
    *object = 0;
    return self->buffers[index]->QueryInterface(g_MusicBufferInterfaceId, object);
}

BOOL __stdcall IsAnyMusicBufferPlaying(MusicControllerView *self)
{
    if (!self->buffers) return FALSE;
    BOOL result = FALSE;
    for (unsigned int i = 0; i < self->bufferCount; ++i)
    {
        if (self->buffers[i])
        {
            DWORD status = 0;
            self->buffers[i]->GetStatus(&status);
            result |= status & 1;
        }
    }
    return result;
}

namespace
{
template<int Mode, int Scale, int Offset, bool Stop>
__forceinline int StepFade(MusicControllerView *self)
{
    if (self->fadeEnabled != Mode) return 0;
    self->fadeRemaining = static_cast<int>(static_cast<unsigned int>(self->fadeRemaining) - 1);
    if (self->fadeRemaining <= 0)
    {
        self->fadeEnabled = 0;
        if (Stop) self->buffers[0]->Stop();
        return 1;
    }
    int product = static_cast<int>(static_cast<unsigned int>(self->fadeRemaining) *
        static_cast<unsigned int>(Scale));
    int quotient = product / self->fadeInitial;
    int volume = static_cast<int>(static_cast<unsigned int>(quotient) + static_cast<unsigned int>(Offset));
    SetMusicBufferVolume(self, volume);
    return 0;
}
}
int __stdcall StepMusicFadeMode1(MusicControllerView *self) { return StepFade<1, 5000, -5000, true>(self); }
int __stdcall StepMusicFadeMode2(MusicControllerView *self) { return StepFade<2, -5000, 0, false>(self); }
int __stdcall StepMusicFadeMode3(MusicControllerView *self) { return StepFade<3, -1000, 0, false>(self); }
int __stdcall StepMusicFadeMode4(MusicControllerView *self) { return StepFade<4, 1000, -1000, false>(self); }

HRESULT __stdcall BindMusicDecoderMemory(MusicDecoderView *self, SoundTrackRecord *record,
    const unsigned char *data, DWORD bytes)
{
    self->record = record;
    self->memorySize = bytes;
    self->memoryStart = data;
    self->cursor = data;
    self->memoryMode = 1;
    return E_NOTIMPL;
}
DWORD __stdcall MusicDecoderDataSize(MusicDecoderView *self) { return self->dataSize; }
HRESULT __stdcall CloseMusicDecoder(MusicDecoderView *self)
{
    if (self->closeOnStop == 1)
    {
        CloseHandle(self->file);
        self->file = INVALID_HANDLE_VALUE;
    }
    return S_OK;
}
DWORD __stdcall MusicDecoderSavedPosition(MusicDecoderView *self) { return self->savedFilePosition; }
HRESULT __stdcall ConfigureControllerMusicDecoder(MusicControllerView *self, SoundTrackRecord *record, int offset)
{
    return ConfigureMusicDecoder(self->decoder, record, offset);
}
void __stdcall SaveMusicDecoderPosition(MusicDecoderView *self)
{
    self->savedFilePosition = SetFilePointer(self->file, 0, 0, FILE_CURRENT);
}
