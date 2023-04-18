#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "appdata.hpp"
#include "entity.hpp"
#include "mob.hpp"

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_image.h"

#include <string>
#include <memory>
#include <vector>

class Renderer {
public:
    Renderer(const std::shared_ptr<AppData>&);
    ~Renderer();
    void Render(std::vector<std::vector<Entity*>>& ,std::vector<Mob*>&);
    void UpdateWindowTitle(const int&);
    SDL_Texture* CreateTexture(const std::string&);

private:
    SDL_Window *window;
    SDL_Renderer *renderer;

    const std::shared_ptr<AppData>& appdata;
};

#endif /* RENDERER_HPP */