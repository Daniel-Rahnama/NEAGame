#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "appdata.hpp"

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_image.h"

#include <string>
#include <memory>

class Renderer {
public:
    Renderer(const std::shared_ptr<AppData>&);
    ~Renderer();
    void Render();
    void UpdateWindowTitle(const int&);

private:
    SDL_Window *window;
    SDL_Renderer *renderer;

    const std::shared_ptr<AppData>& appdata;
};

#endif /* RENDERER_HPP */