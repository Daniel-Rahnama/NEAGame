#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "mob.hpp"

class Player : public Mob {
public:
    using Mob::Mob;
    void Update(const std::shared_ptr<AppData>&, std::vector<std::vector<Entity*>>&, SDL_Rect&);
};

#endif /* PLAYER_HPP */