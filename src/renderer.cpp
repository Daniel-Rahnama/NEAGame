#include "renderer.hpp"
#include <assert.h>

Renderer::Renderer(const std::shared_ptr<AppData>& appdata) : appdata(appdata) {
    
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        throw "Unable to initialise SDL";
    }

    if(TTF_Init() < 0) {
        throw TTF_GetError();
    }

    window = SDL_CreateWindow("Game", NULL, NULL, NULL, NULL, SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);

    if (!window) {
        throw "Unable to create SDL Window";
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer) {
        throw "Unable to create SDL Renderer";
    }

}

Renderer::~Renderer() {
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Renderer::Render() {
    SDL_RenderClear(renderer);
    
    SDL_Surface* image = SDL_LoadBMP((appdata->Resources() + "/sprites/map.bmp").c_str());
    SDL_SetWindowIcon(window, image);
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_Rect dstrect = { 336, 336, 128, 128 };
    SDL_RenderCopy(renderer, texture, NULL, &dstrect);

    TTF_Font* Arial = TTF_OpenFont((appdata->Resources() + "/fonts/arial.ttf").c_str(), 80);
    assert(Arial != NULL);
    SDL_Color White = {255, 255, 255};
    SDL_Surface* sText = TTF_RenderText_Solid(Arial, "Hello World", White);
    SDL_Texture* tText = SDL_CreateTextureFromSurface(renderer, sText);
    SDL_Rect rText = {0, 0, 400, 100};
    SDL_RenderCopy(renderer, tText, NULL, &rText);

    SDL_RenderPresent(renderer);

    SDL_FreeSurface(sText);
    SDL_DestroyTexture(tText);
}

void Renderer::UpdateWindowTitle(const int& FPS) {
    
}