#include "game.hpp"

#include <thread>
#include <iostream>

Game::Game(const std::shared_ptr<AppData>& appdata) : appdata(appdata) {

}

void Game::Run(const std::shared_ptr<Renderer>& renderer, const std::shared_ptr<Controller>& controller, const std::shared_ptr<Mixer>& mixer) {

    mixer->Play();

    Uint32 Target_Frame_Duration = 1000 / appdata->TargetFPS();

    Uint32 Title_Timestamp = SDL_GetTicks();
    Uint32 Frame_Start;
    Uint32 Frame_End;
    Uint32 Frame_Duration;

    unsigned int Frame_Count;

    bool running = true;

    while (running) {
        Frame_Start = SDL_GetTicks();

        controller->HandleInput(running);

        Update(running);

        renderer->Render();

        Frame_End = SDL_GetTicks();

        Frame_Duration = Frame_End - Frame_Start;

        Frame_Count++;

        if (Frame_End - Title_Timestamp >= 1000) {
            renderer->UpdateWindowTitle(Frame_Count);
            Frame_Count = 0;
            Title_Timestamp = Frame_End;
        }

        if (Frame_Duration < Target_Frame_Duration) {
            SDL_Delay(Target_Frame_Duration - Frame_Duration);
        }
    }
}

void Game::Update(bool& running) {

}