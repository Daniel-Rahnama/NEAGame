#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "mob.hpp"

/// @brief Class representing the player
class Player : public Mob {
public:
    using Mob::Mob;

    /// @brief Updates the position of the player
    /// @param appdata AppData shared pointer
    /// @param entities Game map
    /// @param mobs Game mobs
    /// @param camera Rectangle representing the camera (x, y - Camera Position, w, h - Game Map Size)
    void Update(AppData& appdata, std::vector<std::vector<Entity*>>& entities, std::vector<Mob*>& mobs, SDL_Rect& camera);
};

#endif /* PLAYER_HPP */