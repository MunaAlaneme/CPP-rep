//
//  main.cpp
//  CPP rep
//
//  Created by Muna Alaneme on 5/30/25.
//
//  https://github.com/PortAudio/portaudio
//  https://github.com/libsndfile/libsndfile
//  https://github.com/ocornut/imgui
//  https://github.com/libsdl-org/SDL

#include "portaudio.h"
#include "sndfile.h"
#include <vector>
#include <iostream>
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer.h"
#include "SDL.h"

std::vector<float> audioData;
size_t audioIndex = 0;
bool isPlaying = false;

static int audioCallback(const void* inputBuffer, void* outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo* timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void* userData) {
    float* out = (float*)outputBuffer;
    for (unsigned int i = 0; i < framesPerBuffer * 2; ++i) {
        if (audioIndex < audioData.size()) {
            *out++ = audioData[audioIndex++];
        } else {
            *out++ = 0.0f;
        }
    }
    return (audioIndex < audioData.size()) ? paContinue : paComplete;
}

void loadWav(const char* filename) {
    SF_INFO sfInfo;
    SNDFILE* file = sf_open(filename, SFM_READ, &sfInfo);
    if (!file) {
        std::cerr << "Failed to open WAV file\n";
        return;
    }

    audioData.resize(sfInfo.frames * sfInfo.channels);
    sf_readf_float(file, audioData.data(), sfInfo.frames);
    sf_close(file);

    audioIndex = 0;
    std::cout << "Loaded " << sfInfo.frames << " frames\n";
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    SDL_Window* window = SDL_CreateWindow("WAV Player", SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, 400, 200, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    ImGui::CreateContext();
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer_Init(renderer);

    Pa_Initialize();
    PaStream* stream;
    Pa_OpenDefaultStream(&stream, 0, 2, paFloat32, 44100, 256, audioCallback, nullptr);

    loadWav("sample.wav");  // Replace with your stereo WAV file

    bool quit = false;
    SDL_Event event;

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT) quit = true;
        }

        ImGui_ImplSDLRenderer_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Audio Controls");
        if (ImGui::Button("Play")) {
            audioIndex = 0;
            Pa_StartStream(stream);
            isPlaying = true;
        }
        if (ImGui::Button("Stop")) {
            Pa_StopStream(stream);
            isPlaying = false;
        }
        ImGui::Text("Status: %s", isPlaying ? "Playing" : "Stopped");
        ImGui::End();

        ImGui::Render();
        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
        SDL_RenderPresent(renderer);
    }

    Pa_StopStream(stream);
    Pa_CloseStream(stream);
    Pa_Terminate();

    ImGui_ImplSDLRenderer_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}


//int main(int argc, const char * argv[]) {
    // insert code here...
  //  std::cout << "Hello, World!\n";
    //return 0;
//}
