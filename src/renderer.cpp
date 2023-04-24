#include "renderer.hpp"
#include <assert.h>
#include <iostream>

#include "entity.hpp"
#include "mob.hpp"

Renderer::Renderer(const std::shared_ptr<AppData>& appdata) : appdata(appdata) {
    
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        throw SDL_GetError();
    }

    if(TTF_Init() < 0) {
        throw TTF_GetError();
    }

    window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, appdata->Width(), appdata->Height(), (appdata->Fullscreen() ? SDL_WINDOW_FULLSCREEN_DESKTOP : SDL_WINDOW_SHOWN));

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

void Renderer::Render(std::vector<std::vector<Entity*>>& entities, std::vector<Mob*>& mobs, const SDL_Rect& camera) {
    SDL_RenderClear(renderer);

    std::vector<SDL_Rect> dstrects;

    for (std::vector<Entity*>& l : entities) {
        int i = 0;
        for (Entity*& e : l) {
            if ((camera.x < e->DSTRect().x + e->DSTRect().w) && (camera.x + appdata->Width() > e->DSTRect().x)
                && (camera.y < e->DSTRect().y + e->DSTRect().h) && (camera.y + appdata->Height() > e->DSTRect().y)) {
                dstrects.emplace_back(SDL_Rect{ e->DSTRect().x - camera.x, e->DSTRect().y - camera.y, e->DSTRect().w, e->DSTRect().h });
                SDL_RenderCopy(renderer, e->Spritesheet(), &e->SRCRect(), &dstrects.back());

            }
        }
    }

    for (Mob*& m : mobs) {
        if ((camera.x < m->DSTRect().x + m->DSTRect().w) && (camera.x + appdata->Width() > m->DSTRect().x)
            && (camera.y < m->DSTRect().y + m->DSTRect().h) && (camera.y + appdata->Height() > m->DSTRect().y)) {
            dstrects.emplace_back(SDL_Rect{ m->DSTRect().x - camera.x, m->DSTRect().y - camera.y, m->DSTRect().w, m->DSTRect().h });
            SDL_RenderCopy(renderer, m->Spritesheet(), &m->SRCRect(), &dstrects.back());
            SDL_RenderDrawLine(renderer, m->hitbox.x - camera.x, m->hitbox.y - camera.y, m->hitbox.x + m->hitbox.w - camera.x, m->hitbox.y + m->hitbox.h - camera.y);
        }
    }

    SDL_RenderPresent(renderer);
}

void Renderer::UpdateWindowTitle(const int& FPS) {
    SDL_SetWindowTitle(window, ("Game - FPS: " + std::to_string(FPS)).c_str());
}

SDL_Texture* Renderer::CreateTexture(const std::string& spritesheet) {
    return SDL_CreateTextureFromSurface(renderer, IMG_Load((spritesheet).c_str()));
}
