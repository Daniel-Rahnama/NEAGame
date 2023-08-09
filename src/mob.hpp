#ifndef MOB_HPP
#define MOB_HPP

#include "entity.hpp"
#include "appdata.hpp"

#include <memory>

/// @brief State of the mob
typedef enum {
    /// @brief Player facing DOWN
    UP = 0x0000,
    /// @brief Player facing UP
    LEFT = 0x0001,
    /// @brief Player facing LEFT
    DOWN = 0x0002,
    /// @brief Player facing RIGHT
    RIGHT = 0x0003,
    /// @brief Player MOVING
    MOVING = 0x0004,
    /// @brief Player ATTACKING
    ATTACKING = 0x0008,
    /// @brief Player DEAD
    DEAD = 0x8000,
} State;

/// @brief Base class for all mobs
class Mob : public Entity {
public:
    /// @brief Sets the spritesheet, source rectangle, destination rectangle, and layer of the mob, creates the hitbox
    /// @param spritesheet Pointer to spritesheet of the entity
    /// @param state State of the mob
    /// @param dstrect Destination rectangle of the entity on the screen
    /// @param layer Layer the mob exists on
    Mob(SDL_Texture* spritesheet, uint16_t state, SDL_Rect dstrect, unsigned int layer);

    /// @brief Animates the mob
    void UpdateAnimation();

    /// @brief Updates the position of the mob
    /// @param entities Game map
    /// @param mobs Game mobs
    /// @param EvaluatePlayerCollision Function to evaluate whether a rectangle collides with the Player
    /// @param camera Rectangle representing the camera (x, y - Camera Position, w, h - Game Map Size)
    void Update(std::vector<std::vector<Entity*>>& entities, std::vector<Mob*>& mobs, std::function<bool (const SDL_Rect&, const int&)>& EvaluatePlayerCollision, SDL_Rect& camera);

    /// @brief Getter for the layer the mob exists on
    /// @return Reference to the int representing the layer the mob exists on
    const unsigned int& Layer() const;

    /// @brief Getter for the health of the mob
    /// @return Const reference to the double representing the health of the mob
    const double& Health() const;

    /// @brief Rectangle representing the hitbox of the mob
    SDL_Rect hitbox;

    /// @brief State of the mob
    uint16_t state;

    /// @brief Mob Hit
    /// @param damage Damage dealt to the mob when hit
    void Hit(const double& damage);
protected:
    /// @brief Collision detection for the mob's hitbox and an entity
    /// @param e Entity to check collision with
    /// @return 1 if collision, 0 if no collision
    bool Collision(Entity*& e);

    /// @brief Collision detection for the mob's hitbox and a rectangle
    /// @param m Mob to check collision with
    /// @param r Rectangle to check collision with
    /// @return 1 if collision, 0 if no collision
    bool Collision(Mob*& m, const SDL_Rect& r);

    /// @brief Layer the mob exists on
    unsigned int layer;

    /// @brief Mob health out 100.00
    double health;
};

#endif /* MOB_HPP */