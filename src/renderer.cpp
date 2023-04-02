#include "renderer.hpp"
#include <assert.h>
#include <iostream>

#include "entity.hpp"
#include "mob.hpp"

Renderer::Renderer(const std::shared_ptr<AppData>& appdata) : appdata(appdata) {
    
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        throw "Unable to initialise SDL";
    }

    if(TTF_Init() < 0) {
        throw TTF_GetError();
    }

    window = SDL_CreateWindow("Game", 0, 0, appdata->Width(), appdata->Height(), SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);

    if (!window) {
        throw "Unable to create SDL Window";
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer) {
        throw "Unable to create SDL Renderer";
    }

    SDL_SetWindowIcon(window, IMG_Load((appdata->Resources() + "/sprites/map.png").c_str()));
}

Renderer::~Renderer() {
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Renderer::Render(std::vector<Mob*>& entities) {
    SDL_RenderClear(renderer);

    for (Mob* e : entities) {
        SDL_RenderCopy(renderer, e->Spritesheet(), &e->SRCRect(), &e->DSTRect());
    }

    SDL_RenderPresent(renderer);
}

void Renderer::UpdateWindowTitle(const int& FPS) {
    
}

SDL_Texture* Renderer::CreateTexture(const std::string& spritesheet) {
    return SDL_CreateTextureFromSurface(renderer, IMG_Load((appdata->Resources() + spritesheet).c_str()));
}
