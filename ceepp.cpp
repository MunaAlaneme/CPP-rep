#include "portaudio.hpp"
#include <vector>
#include <iostream>

std::vector<float> resampledAudio; // global buffer
size_t playbackIndex = 0;

static int audioCallback(const void* inputBuffer, void* outputBuffer,
                            unsigned long framesPerBuffer,
                            const PaStreamCallbackTimeInfo* timeInfo,
                            PaStreamCallbackFlags statusFlags,
                            void* userData) {
    float* out = static_cast<float*>(outputBuffer);
    for (unsigned int i = 0; i < framesPerBuffer; ++i) {
        if (playbackIndex < resampledAudio.size()) {
            *out++ = resampledAudio[playbackIndex++]; // mono
        } else {
            *out++ = 0.0f; // silence after end
        }
    }
    return playbackIndex < resampledAudio.size() ? paContinue : paComplete;
}

int main() {
    // Load your resampled data (as float samples in [-1.0, 1.0])
    // You can use the resample() function from before here
    // For demonstration, we simulate it with dummy data:
    for (int i = 0; i < 44100; ++i) {
        resampledAudio.push_back(0.2f * sin(2 * 3.141592 * 440 * i / 44100)); // A4 tone
    }

    Pa_Initialize();
    PaStream* stream;
    Pa_OpenDefaultStream(&stream, 0, 1, paFloat32, 44100, 256, audioCallback, nullptr);
    Pa_StartStream(stream);

    while (Pa_IsStreamActive(stream)) Pa_Sleep(100);

    Pa_StopStream(stream);
    Pa_CloseStream(stream);
    Pa_Terminate();

    return 0;
}
