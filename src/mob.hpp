#ifndef MOB_HPP
#define MOB_HPP

#include "entity.hpp"
#include "appdata.hpp"

#include <memory>

enum class Direction {
    NONE,
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class Mob : public Entity {
public:
    Mob(SDL_Texture*&, SDL_Rect, SDL_Rect, unsigned int);

    void UpdateAnimation(const std::shared_ptr<AppData>&);
    void Update(const std::shared_ptr<AppData>&, std::vector<std::vector<Entity*>>&, SDL_Rect&);

    unsigned int& Layer();
    SDL_Rect hitbox;
    Direction direction;
protected:
    bool Collision(Entity*&);

    unsigned int layer;


};

#endif /* MOB_HPP */