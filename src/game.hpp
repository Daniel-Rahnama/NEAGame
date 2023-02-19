#ifndef GAME_HPP
#define GAME_HPP

#include "SDL2/SDL.h"

#include "renderer.hpp"
#include "controller.hpp"

class Game {
public:
    Game(size_t, size_t);
    void Run(const int&, Renderer&, Controller&);
    void Update(bool&);
private:
    size_t game_width;
    size_t game_height;
};

#endif /* GAME_HPP */