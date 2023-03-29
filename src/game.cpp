#include "game.hpp"

#include <thread>
#include <iostream>
#include <vector>

#include "entity.hpp"

Game::Game(const std::shared_ptr<AppData>& appdata) : appdata(appdata) {

}

void Game::Run(const std::shared_ptr<Renderer>& renderer, const std::shared_ptr<Controller>& controller, const std::shared_ptr<Mixer>& mixer) {

    mixer->Play();

    std::vector<Entity*> entities;

    SDL_Texture* t = renderer->CreateTexture("/sprites/c1.png");

    Entity e(t, {64, 704, 64, 64}, {336, 336, 128, 128});

    entities.emplace_back(&e);

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

        if (!(Frame_Count % 5)) {
            entities[0]->SRCRect().x += 64;
            if (entities[0]->SRCRect().x >= 576) entities[0]->SRCRect().x = 64;
        }

        entities[0]->DSTRect().x += 4;
        if (entities[0]->DSTRect().x > appdata->Width()) entities[0]->DSTRect().x = 0;


        renderer->Render(entities);

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