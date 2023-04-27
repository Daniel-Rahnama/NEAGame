#ifndef GAME_HPP
#define GAME_HPP

#include "SDL2/SDL.h"
#include "Tmx.h"

#include "renderer.hpp"
#include "controller.hpp"
#include "mixer.hpp"
#include "appdata.hpp"
#include "mob.hpp"
#include "player.hpp"

#include <string>
#include <memory>
#include <vector>
#include <map>

class Game {
public:
    Game(const std::shared_ptr<AppData>&, const std::shared_ptr<Renderer>&, const std::shared_ptr<Controller>&, const std::shared_ptr<Mixer>&);
    void Run();
    void Update(bool&, unsigned int&, Player&);
private:
    void LoadMap();

    SDL_Rect camera;

    Tmx::Map tileMap;

    struct Tileset {
        Tmx::Tileset* tileset;
        SDL_Texture* texture;
    };

    std::map<std::string, Tileset> tilesets;

    std::vector<std::vector<Entity*>> entities;
    std::vector<Mob*> mobs;
    Player* player;

    const std::shared_ptr<AppData>& appdata;
    const std::shared_ptr<Renderer>& renderer;
    const std::shared_ptr<Controller>& controller;
    const std::shared_ptr<Mixer>& mixer;
};

#endif /* GAME_HPP */