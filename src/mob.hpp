#ifndef MOB_HPP
#define MOB_HPP

#include "entity.hpp"
#include "appdata.hpp"

#include <memory>

/// @brief State of the mob
typedef enum {
    /// @brief Player facing UP
    UP = 0x0001,
    /// @brief Player facing DOWN
    DOWN = 0x0002,
    /// @brief Player facing LEFT
    LEFT = 0x0004,
    /// @brief Player facing RIGHT
    RIGHT = 0x0008,
    /// @brief Player MOVING
    MOVING = 0x0010,
} State;

/// @brief Base class for all mobs
class Mob : public Entity {
public:
    /// @brief Sets the spritesheet, source rectangle, destination rectangle, and layer of the mob, creates the hitbox
    /// @param spritesheet Reference to pointer to spritesheet of the entity
    /// @param srcrect Source rectangle of the entity on the spritesheet
    /// @param dstrect Destination rectangle of the entity on the screen
    /// @param layer Layer the mob exists on
    Mob(SDL_Texture*& spritesheet, SDL_Rect srcrect, SDL_Rect dstrect, unsigned int layer);

    /// @brief Animates the mob
    /// @param appdata AppData shared pointer
    void UpdateAnimation(const std::unique_ptr<AppData>& appdata);

    /// @brief Updates the position of the mob
    /// @param appdata AppData shared pointer
    /// @param entities Game map
    /// @param camera Rectangle representing the camera (x, y - Camera Position, w, h - Game Map Size)
    void Update(const std::unique_ptr<AppData>& appdata, std::vector<std::vector<Entity*>>& entities, SDL_Rect& camera);

    /// @brief Getter for the layer the mob exists on
    /// @return Reference to the int representing the layer the mob exists on
    unsigned int& Layer();

    /// @brief Rectangle representing the hitbox of the mob
    SDL_Rect hitbox;

    /// @brief Direction the mob is moving
    uint16_t state;
protected:
    /// @brief Collision detection for the mob's hitbox and an entity
    /// @param e Entity to check collision with
    /// @return 1 if collision, 0 if no collision
    bool Collision(Entity*& e);

    /// @brief Layer the mob exists on
    unsigned int layer;
};

#endif /* MOB_HPP */