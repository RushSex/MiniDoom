// audio_win32.cpp — waveOut: старейший и самый простой звук Windows
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <mmsystem.h>

#include "audio.hpp"

#include <array>
#include <cstdint>

namespace doom {

AudioOut::AudioOut(Mixer& mixer)
    : mixer_(mixer),
      thread_([this](std::stop_token stop) { run(stop); }) {}

void AudioOut::run(std::stop_token stop) {
    WAVEFORMATEX fmt{};
    fmt.wFormatTag = WAVE_FORMAT_PCM;
    fmt.nChannels = 1;                       // моно
    fmt.nSamplesPerSec = kSampleRate;
    fmt.wBitsPerSample = 16;
    fmt.nBlockAlign = 2;                     // байт на сэмпл
    fmt.nAvgBytesPerSec = kSampleRate * 2;
    const HANDLE done = CreateEventW(nullptr, FALSE, FALSE, nullptr);
    HWAVEOUT out = nullptr;
    if (waveOutOpen(&out, WAVE_MAPPER, &fmt,
                    reinterpret_cast<DWORD_PTR>(done), 0,
                    CALLBACK_EVENT) != MMSYSERR_NOERROR) {
        CloseHandle(done);
        return;                         // нет звука — играем молча
    }
    // Три буфера по 512 сэмплов (23 мс): один звучит,
    // второй ждёт очереди, третий мы заполняем.
    constexpr std::size_t kChunk = 512;
    std::array<std::array<std::int16_t, kChunk>, 3> data{};
    std::array<WAVEHDR, 3> hdr{};
    for (std::size_t i = 0; i < hdr.size(); ++i) {
        hdr[i].lpData = reinterpret_cast<LPSTR>(data[i].data());
        hdr[i].dwBufferLength = static_cast<DWORD>(sizeof(data[i]));
        waveOutPrepareHeader(out, &hdr[i], sizeof(WAVEHDR));
        hdr[i].dwFlags |= WHDR_DONE;    // «свободен» — заполнить
    }
    while (!stop.stop_requested()) {
        for (std::size_t i = 0; i < hdr.size(); ++i) {
            if ((hdr[i].dwFlags & WHDR_DONE) != 0) {
                mixer_.mix(data[i]);
                waveOutWrite(out, &hdr[i], sizeof(WAVEHDR));
            }
        }
        WaitForSingleObject(done, 20);  // доиграл буфер — разбудят
    }
    waveOutReset(out);                       // стоп и вернуть буферы
    for (WAVEHDR& h : hdr) {
        waveOutUnprepareHeader(out, &h, sizeof(WAVEHDR));
    }
    waveOutClose(out);
    CloseHandle(done);
}

} // namespace doom
