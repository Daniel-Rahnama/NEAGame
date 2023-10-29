#include "renderer.hpp"

#include <assert.h>
#include <iostream>
#include <algorithm>
#include <functional>

#include "entity.hpp"
#include "mob.hpp"

Renderer::Renderer(AppData& appdata) : appdata(appdata) {
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        throw SDL_GetError();
    }

    window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, appdata.Width(), appdata.Height(), (appdata.Fullscreen() ? SDL_WINDOW_FULLSCREEN_DESKTOP : SDL_WINDOW_SHOWN));

    if (!window) {
        throw "Unable to create SDL Window";
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer) {
        throw "Unable to create SDL Renderer";
    }

    SDL_SetWindowIcon(window, IMG_Load((appdata.Resources() + "/sprites/map.png").c_str()));

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
}

Renderer::~Renderer() {
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Renderer::Render(std::vector<std::vector<Entity*>>& entities, std::vector<Mob*>& mobs, Player*& player, const SDL_Rect& camera) {
    SDL_RenderClear(renderer);

    std::vector<SDL_Rect> dstrects;

    for (std::vector<Entity*>& l : entities) {
        for (Entity*& e : l) {
            if ((camera.x < e->DSTRect().x + e->DSTRect().w) && (camera.x + appdata.Width() > e->DSTRect().x)
                && (camera.y < e->DSTRect().y + e->DSTRect().h) && (camera.y + appdata.Height() > e->DSTRect().y)) {
                dstrects.emplace_back(SDL_Rect{ e->DSTRect().x - camera.x, e->DSTRect().y - camera.y, e->DSTRect().w, e->DSTRect().h });
                SDL_RenderCopy(renderer, e->Spritesheet(), &e->SRCRect(), &dstrects.back());
            }
        }
    }

    mobs = SortMobs(mobs);

    bool playerRendered = false;

    static std::function<void ()> renderPlayer = [&player, &playerRendered, &camera, &dstrects, this]() -> void {
        dstrects.emplace_back(SDL_Rect{ player->DSTRect().x - camera.x, player->DSTRect().y - camera.y, player->DSTRect().w, player->DSTRect().h });
        SDL_RenderCopy(renderer, player->Spritesheet(), &player->SRCRect(), &dstrects.back());
        SDL_RenderDrawLine(renderer, player->hitbox.x - camera.x, player->hitbox.y - camera.y, player->hitbox.x + player->hitbox.w - camera.x, player->hitbox.y + player->hitbox.h - camera.y);
        playerRendered = true;
    };

    for (Mob*& m : mobs) {
        if ((camera.x < m->DSTRect().x + m->DSTRect().w) && (camera.x + appdata.Width() > m->DSTRect().x)
            && (camera.y < m->DSTRect().y + m->DSTRect().h) && (camera.y + appdata.Height() > m->DSTRect().y)) {
            if ((m->DSTRect().y + (m->DSTRect().w / 2) > player->DSTRect().y + (player->DSTRect().w / 2)) && !playerRendered) {
                renderPlayer();
            }

            dstrects.emplace_back(SDL_Rect{ m->DSTRect().x - camera.x, m->DSTRect().y - camera.y, m->DSTRect().w, m->DSTRect().h });
            SDL_RenderCopy(renderer, m->Spritesheet(), &m->SRCRect(), &dstrects.back());
            SDL_RenderDrawLine(renderer, m->hitbox.x - camera.x, m->hitbox.y - camera.y, m->hitbox.x + m->hitbox.w - camera.x, m->hitbox.y + m->hitbox.h - camera.y);
            
            dstrects.emplace_back(SDL_Rect{ (m->hitbox.x - camera.x), (m->hitbox.y - camera.y - 80), (int)(64 * (m->Health() / 100)), 8 });
            SDL_SetRenderDrawColor(renderer, 0xff, 0x00, 0x00, 0xaa);
            SDL_RenderFillRect(renderer, &dstrects.back());
            SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
        }
    }

    if (!playerRendered) renderPlayer();
    
    SDL_Rect hBarOutline { (appdata.Width() - 800) / 2, appdata.Height() - 100, 800, 50 };
    SDL_Rect hBar { (appdata.Width() - 800) / 2, appdata.Height() - 100, (int)(8 * (player->Health())), 50 };

    SDL_SetRenderDrawColor(renderer, 0xff, 0x00, 0x00, 0xaa);
    SDL_RenderFillRect(renderer, &hBar);

    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
    SDL_RenderDrawRect(renderer, &hBarOutline);

    SDL_RenderPresent(renderer);
}

void Renderer::UpdateWindowTitle(const int& FPS) {
    SDL_SetWindowTitle(window, ("Game - " + appdata.PlayerUsername() + " - FPS: " + std::to_string(FPS)).c_str());
}

SDL_Texture* Renderer::CreateTexture(const std::string& spritesheet) {
    return SDL_CreateTextureFromSurface(renderer, IMG_Load((spritesheet).c_str()));
}

std::vector<Mob*> Renderer::SortMobs(const std::vector<Mob*>& mobs) {
    std::vector<Mob*> merged;

    if (mobs.size() <= 1) {
        return mobs;
    } else {
        int midpoint = (mobs.size() - 1) / 2;
        int sizeL = midpoint + 1;
        int sizeR = midpoint + (mobs.size() % 2 == 0 ? 1 : 0);

        std::vector<Mob*> left, right;

        for (int i = 0; i < sizeL; i++) {
            left[i] = mobs[i];
        }

        for (int i = 0; i < sizeR; i++) {
            right[i] = mobs[sizeL + i];
        }

        left = SortMobs(left);
        right = SortMobs(right);

        merged = MergeMobs(left, right);
    }

    return merged;
}

std::vector<Mob*> Renderer::MergeMobs(const std::vector<Mob*>& left, const std::vector<Mob*>& right) {
    std::vector<Mob*> merged;

    int indexLeft = 0;
    int indexRight = 0;
    int indexMerged = 0;

    while ((indexLeft < left.size()) && (indexRight < right.size())) {
        if (left[indexLeft]->DSTRect().y > right[indexRight]->DSTRect().y) {
            merged[indexMerged] = left[indexLeft];
            indexLeft++;
            indexMerged++;
        } else {
            merged[indexMerged] = right[indexRight];
            indexRight++;
            indexMerged++;
        }
    }

    while (indexLeft < left.size()) {
        merged[indexMerged] = left[indexLeft];
        indexLeft++;
        indexMerged++;
    }

    while (indexRight < right.size()) {
        merged[indexMerged] = right[indexRight];
        indexRight++;
        indexMerged++;
    }

    return merged;
}