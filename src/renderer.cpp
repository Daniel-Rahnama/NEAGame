#include "renderer.hpp"
#include <assert.h>

Renderer::Renderer(size_t width, size_t height, const char* resources) : screen_width(width), screen_height(height), resources(resources) {
    
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        throw "Unable to initialise SDL";
    }

    if(TTF_Init() < 0) {
        throw TTF_GetError();
    }

    window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_width, screen_height, SDL_WINDOW_SHOWN);

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
    SDL_Surface* image = SDL_LoadBMP((resources + "/sprites/map.bmp").c_str());
    SDL_SetWindowIcon(window, image);
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_Rect dstrect = { 336, 336, 128, 128 };
    SDL_RenderCopy(renderer, texture, NULL, &dstrect);

    TTF_Font* Arial = TTF_OpenFont((resources + "/fonts/arial.ttf").c_str(), 80);
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