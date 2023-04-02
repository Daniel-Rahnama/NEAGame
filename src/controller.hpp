#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include "SDL2/SDL.h"

#include "mob.hpp"

class Controller {
public:
    void HandleInput(bool&, Direction&);
};

#endif /* CONTROLLER_HPP */