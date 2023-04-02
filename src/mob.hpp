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
    void Update(const std::shared_ptr<AppData>&);

    Direction direction;
private:
    void MoveUp(const unsigned int&);
    void MoveDown(const unsigned int&);
    void MoveLeft(const unsigned int&);
    void MoveRight(const unsigned int&);
};

#endif /* MOB_HPP */