#include "game.hpp"

#include <thread>

Game::Game(size_t width, size_t height, const char* resources) : game_width(width), game_height(height), resources(resources) {
    Mixer m(resources);
    m.Play();
}

void Game::Run(const int& Target_Frame_Duration, Renderer& renderer, Controller& controller) {

    Uint32 Title_Timestamp = SDL_GetTicks();
    Uint32 Frame_Start;
    Uint32 Frame_End;
    Uint32 Frame_Duration;

    bool running = true;

    while (running) {
        Frame_Start = SDL_GetTicks();

        controller.HandleInput(running);

        Update(running);

        renderer.Render();

        Frame_End = SDL_GetTicks();

        Frame_Duration = Frame_End - Frame_Start;

        if (Frame_Duration < Target_Frame_Duration) {
            SDL_Delay(Target_Frame_Duration - Frame_Duration);
        }
    }
}

void Game::Update(bool& running) {

}