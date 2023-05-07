#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include "SDL2/SDL.h"

#include "mob.hpp"

/// @brief Handles user input
class Controller {
public:
    /// @brief Handles user input
    /// @param running Reference to the running variable in game loop
    /// @param state State reference to the state of the player
    void HandleInput(bool& running, uint16_t& state);
};

#endif /* CONTROLLER_HPP */