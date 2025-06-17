//
//  main.cpp
//  CPP rep
//
//  Created by Muna Alaneme on 5/30/25.
//
//  https://github.com/raysan5/raylib
//  https://github.com/libsdl-org/SDL

#include "raylib.h"
extern "C" {
    #include <SDL3/SDL.h>
}

int main() {
    // Initialize SDL (without creating a window)
    if (SDL_Init(SDL_INIT_GAMEPAD | SDL_INIT_EVENTS) != 0) {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return -1;
    }

    // Initialize Raylib window and OpenGL context
    InitWindow(800, 600, "Raylib with SDL3 Input");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) CloseWindow();
            // handle more SDL input events here
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Using Raylib + SDL3", 190, 200, 20, LIGHTGRAY);
        EndDrawing();
    }

    CloseWindow();
    SDL_Quit();
    return 0;
}
