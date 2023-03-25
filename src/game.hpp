#ifndef GAME_HPP
#define GAME_HPP

#include "SDL2/SDL.h"

#include "renderer.hpp"
#include "controller.hpp"
#include "mixer.hpp"
#include "appdata.hpp"

#include <string>
#include <memory>

class Game {
public:
    Game(const std::shared_ptr<AppData>&);
    void Run(const std::shared_ptr<Renderer>&, const std::shared_ptr<Controller>&);
    void Update(bool&);
private:
    const std::shared_ptr<AppData>& appdata;
};

#endif /* GAME_HPP */