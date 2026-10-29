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
    if (index != self->bufferCount)
        return self->buffers[index];
    return self->buffers[static_cast<unsigned int>(rand()) % self->bufferCount];
}

HRESULT __stdcall RefillMusicBuffer(MusicSoundBaseView *self, SoundBufferView *buffer, int loop)
{
    void *data = 0;
    DWORD bytes = 0, read = 0;
    if (!buffer) return NotInitialized;
    HRESULT result = RestoreBuffer(buffer, 0);
    if (FAILED(result)) return result;
    result = buffer->Lock(0, self->bufferBytes, &data, &bytes, 0, 0, 0);
    if (FAILED(result)) return result;
    ResetDecoder(self->decoder, 0, false);
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
                result = ResetDecoder(self->decoder, 0, false);
                if (FAILED(result)) return result;
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
    self->fadeInitial = self->fadeRemaining = self->fadeEnabled = 0;
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
    HRESULT result = S_OK;
    self->active = 0;
    self->paused = 0;
    for (unsigned int i = 0; i < self->bufferCount; ++i)
    {
        result |= self->buffers[i]->Stop();
        result |= self->buffers[i]->SetCurrentPosition(0);
    }
    self->fadeEnabled = 0;
    if (closeFile)
    {
        MusicDecoderView *decoder = self->decoder;
        if (decoder->closeOnStop == 1)
        {
            CloseHandle(decoder->file);
            decoder->file = INVALID_HANDLE_VALUE;
        }
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
    if (!self->buffers) return NotInitialized;
    if (!self->paused) return NotInitialized;
    self->paused = 0;
    MusicDecoderView *decoder = self->decoder;
    ConfigureMusicDecoder(decoder, decoder->record, decoder->savedFilePosition);
    SoundBufferView *buffer = self->buffers[0];
    self->active = 1;
    return buffer->Play(0, self->playPriority, self->playFlags);
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
            bytes = static_cast<DWORD>(reinterpret_cast<ULONG_PTR>(self->memoryStart) - cursor + self->memorySize);
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
    BOOL result = FALSE;
    if (!self->buffers) return result;
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

HRESULT __stdcall ConfigurePrimarySoundFormat(SoundDeviceOwnerView *self)
{
    SoundBufferView *buffer = 0;
    if (!self->device) return NotInitialized;
    SoundBufferDescription description;
    memset(&description, 0, sizeof(description));
    description.size = sizeof(description);
    description.flags = 1;
    description.bytes = 0;
    description.format = 0;
    HRESULT result = self->device->CreateSoundBuffer(&description, &buffer, 0);
    if (FAILED(result)) return result;
    SoundWaveFormatView format;
    memset(&format, 0, sizeof(format));
    format.formatTag = 1;
    format.channels = 2;
    format.samplesPerSecond = 44100;
    format.bitsPerSample = 16;
    format.blockAlign = 4;
    format.bytesPerSecond = 176400;
    result = buffer->SetFormat(&format);
    if (FAILED(result)) return result;
    if (buffer) buffer->Release();
    return S_OK;
}

HRESULT __stdcall RecreateMusicBuffers(MusicControllerView *self)
{
    self->active = 0;
    for (unsigned int i = 0; i < self->bufferCount; ++i)
    {
        if (self->buffers[i])
        {
            self->buffers[i]->Release();
            self->buffers[i] = 0;
        }
    }
    if (self->buffers)
    {
        delete[] self->buffers;
        self->buffers = 0;
    }
    SoundNotificationView *notification = 0;
    self->buffers = new SoundBufferView *[self->bufferCount];
    for (unsigned int i = 0; i < self->bufferCount; ++i)
    {
        HRESULT result = self->deviceOwner->device->CreateSoundBuffer(
            &self->bufferDescription, &self->buffers[i], 0);
        if (FAILED(result)) return E_FAIL;
        result = self->buffers[i]->QueryInterface(g_MusicNotificationInterfaceId,
            reinterpret_cast<void **>(&notification));
        if (FAILED(result)) return E_FAIL;
        SoundNotificationPosition *positions = new SoundNotificationPosition[16];
        if (!positions) return E_OUTOFMEMORY;
        for (DWORD j = 0; j < 16; ++j)
        {
            positions[j].offset = self->notificationBytes * (j + 1) - 1;
            positions[j].event = self->notificationEvent;
        }
        result = notification->SetNotificationPositions(16, positions);
        if (FAILED(result))
        {
            if (notification)
            {
                notification->Release();
                notification = 0;
            }
            delete[] positions;
            return E_FAIL;
        }
        if (notification)
        {
            notification->Release();
            notification = 0;
        }
        delete[] positions;
    }
    return S_OK;
}


void __stdcall InitializeSoundDeviceOwner(SoundDeviceOwnerView *self)
{
    self->device = 0;
}
void __stdcall ReleaseSoundDevice(SoundDeviceOwnerView *self)
{
    if (self->device)
    {
        self->device->Release();
        self->device = 0;
    }
}

// Target calls the DSOUND ordinal11 import thunk (DirectSoundCreate8).
extern "C" HRESULT WINAPI DirectSoundCreate8(const GUID *device, SoundDeviceView **sound, SoundUnknownView *outer);
HRESULT __stdcall InitializeSoundDevice(SoundDeviceOwnerView *self, HWND window)
{
    if (self->device)
    {
        self->device->Release();
        self->device = 0;
    }
    HRESULT result = DirectSoundCreate8(0, &self->device, 0);
    if (FAILED(result)) return result;
    result = self->device->SetCooperativeLevel(window, 2);
    if (FAILED(result)) return result;
    ConfigurePrimarySoundFormat(self);
    return S_OK;
}


__forceinline MusicDecoderView::MusicDecoderView()
{
    record = 0;
    unknown000 = 0;
    dataSize = 0;
    memoryMode = 0;
}
__forceinline MusicDecoderView::~MusicDecoderView()
{
    if (closeOnStop == 1)
    {
        CloseHandle(file);
        file = INVALID_HANDLE_VALUE;
    }
}

MusicSoundBaseView::MusicSoundBaseView(SoundBufferView **initial, DWORD bytes,
    DWORD count, MusicDecoderView *wave)
{
    buffers = new SoundBufferView *[count];
    for (DWORD i = 0; i < count; ++i) buffers[i] = initial[i];
    bufferBytes = bytes;
    bufferCount = count;
    decoder = wave;
    RefillMusicBuffer(this, buffers[0], 0);
    buffers[0]->SetCurrentPosition(0);
    active = 0;
    paused = 0;
}
MusicSoundBaseView::~MusicSoundBaseView()
{
    for (DWORD i = 0; i < bufferCount; ++i)
    {
        if (buffers[i])
        {
            buffers[i]->Release();
            buffers[i] = 0;
        }
    }
    if (buffers)
    {
        delete[] buffers;
        buffers = 0;
    }
    if (decoder)
    {
        delete decoder;
        decoder = 0;
    }
}
MusicControllerView::MusicControllerView(SoundBufferView *buffer, DWORD bytes,
    MusicDecoderView *wave, DWORD notifyBytes)
    : MusicSoundBaseView(&buffer, bytes, 1, wave)
{
    lastPlayPosition = 0;
    playProgress = 0;
    nextWriteOffset = 0;
    fillSilence = 0;
    notificationBytes = notifyBytes;
}
inline MusicControllerView::~MusicControllerView() {}

HRESULT SoundDeviceOwnerView::CreateFileMusicStream(MusicControllerView **output,
    GUID algorithm, DWORD notificationBytes, HANDLE event, SoundTrackRecord *record)
{
    SoundDeviceOwnerView *owner = this;
    if (!owner->device) return NotInitialized;
    SoundBufferView *buffer = 0;
    SoundNotificationView *notification = 0;
    MusicDecoderView *decoder = new MusicDecoderView;
    decoder->memoryMode = 0;
    decoder->closeOnStop = 1;
    if (OpenMusicDecoderFile(decoder, g_MusicArchiveFilename, record) != S_OK)
    {
        delete decoder;
        return E_FAIL;
    }
    SoundBufferDescription description;
    memset(&description, 0, sizeof(description));
    description.size = sizeof(description);
    description.flags = 0x18188;
    description.bytes = notificationBytes * 16;
    description.algorithm = algorithm;
    description.format = reinterpret_cast<SoundWaveFormatView *>(&decoder->record->formatTag);
    HRESULT result = owner->device->CreateSoundBuffer(&description, &buffer, 0);
    if (FAILED(result)) return E_FAIL;
    result = buffer->QueryInterface(g_MusicNotificationInterfaceId,
        reinterpret_cast<void **>(&notification));
    if (FAILED(result)) return E_FAIL;
    SoundNotificationPosition *positions = new SoundNotificationPosition[16];
    if (!positions) return E_OUTOFMEMORY;
    for (DWORD i = 0; i < 16; ++i)
    {
        positions[i].offset = notificationBytes * (i + 1) - 1;
        positions[i].event = event;
    }
    result = notification->SetNotificationPositions(16, positions);
    if (FAILED(result))
    {
        if (notification)
        {
            notification->Release();
            notification = 0;
        }
        delete[] positions;
        return E_FAIL;
    }
    if (notification)
    {
        notification->Release();
        notification = 0;
    }
    delete[] positions;
    *output = new MusicControllerView(buffer, description.bytes, decoder, notificationBytes);
    (*output)->bufferDescription = description;
    (*output)->deviceOwner = owner;
    (*output)->notificationEvent = event;
    (*output)->busy = 0;
    return S_OK;
}
HRESULT SoundDeviceOwnerView::CreateMemoryMusicStream(MusicControllerView **output,
    const unsigned char *data, DWORD bytes, SoundTrackRecord *record, GUID algorithm,
    DWORD notificationBytes, HANDLE event)
{
    SoundDeviceOwnerView *owner = this;
    if (!owner->device) return NotInitialized;
    SoundBufferView *buffer = 0;
    SoundNotificationView *notification = 0;
    MusicDecoderView *decoder = new MusicDecoderView;
    BindMusicDecoderMemory(decoder, record, data, bytes);
    SoundBufferDescription description;
    memset(&description, 0, sizeof(description));
    description.size = sizeof(description);
    description.flags = 0x18188;
    description.bytes = notificationBytes * 16;
    description.algorithm = algorithm;
    description.format = reinterpret_cast<SoundWaveFormatView *>(&decoder->record->formatTag);
    HRESULT result = owner->device->CreateSoundBuffer(&description, &buffer, 0);
    if (FAILED(result)) return E_FAIL;
    result = buffer->QueryInterface(g_MusicNotificationInterfaceId,
        reinterpret_cast<void **>(&notification));
    if (FAILED(result)) return E_FAIL;
    SoundNotificationPosition *positions = new SoundNotificationPosition[16];
    if (!positions) return E_OUTOFMEMORY;
    for (DWORD i = 0; i < 16; ++i)
    {
        positions[i].offset = notificationBytes * (i + 1) - 1;
        positions[i].event = event;
    }
    result = notification->SetNotificationPositions(16, positions);
    if (FAILED(result))
    {
        if (notification)
        {
            notification->Release();
            notification = 0;
        }
        delete[] positions;
        return E_FAIL;
    }
    if (notification)
    {
        notification->Release();
        notification = 0;
    }
    delete[] positions;
    *output = new MusicControllerView(buffer, description.bytes, decoder, notificationBytes);
    (*output)->bufferDescription = description;
    (*output)->deviceOwner = owner;
    (*output)->notificationEvent = event;
    (*output)->busy = 0;
    return S_OK;
}
