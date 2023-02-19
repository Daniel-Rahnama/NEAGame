#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "SDL2/SDL.h"

class Renderer {
public:
    Renderer(size_t, size_t);
    ~Renderer();
    void Render();

private:
    SDL_Window *window;
    SDL_Renderer *renderer;

    size_t screen_width;
    size_t screen_height;
};

#endif /* RENDERER_HPP */