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
    using Entity::Entity;

    void UpdateAnimation(const std::shared_ptr<AppData>&);
    void Update(const std::shared_ptr<AppData>&, std::vector<std::vector<Entity*>>&);

    Direction direction;
    unsigned int layer;
private:

};

#endif /* MOB_HPP */