#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_image.h"

#include <string>

class Renderer {
public:
    Renderer(size_t, size_t, const char*);
    ~Renderer();
    void Render();

private:
    SDL_Window *window;
    SDL_Renderer *renderer;

    size_t screen_width;
    size_t screen_height;

    std::string resources;
};

#endif /* RENDERER_HPP */