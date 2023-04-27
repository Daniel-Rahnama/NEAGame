#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <string>
#include "SDL2/SDL.h"

/// @brief Base class for all entities
class Entity {
public:
    /// @brief Sets the spritesheet, source rectangle, and destination rectangle of the entity
    /// @param spritesheet Reference to pointer to spritesheet of the entity
    /// @param srcrect Source rectangle of the entity on the spritesheet
    /// @param dstrect Destination rectangle of the entity on the screen
    Entity(SDL_Texture*& spritesheet, SDL_Rect srcrect, SDL_Rect dstrect);

    /// @brief Destructor
    ~Entity();

    /// @brief Spritesheet Getter
    /// @return Spritesheet of the entity
    SDL_Texture*& Spritesheet();

    /// @brief Source Rectangle Getter
    /// @return Source rectangle of the entity on the spritesheet
    SDL_Rect& SRCRect();

    /// @brief Destination Rectangle Getter
    /// @return Destination rectangle of the entity on the screen
    SDL_Rect& DSTRect();
protected:
    /// @brief Reference to pointer to spritesheet of the entity
    SDL_Texture*& spritesheet;

    /// @brief Source rectangle of the entity on the spritesheet
    SDL_Rect srcrect; 

    /// @brief Destination rectangle of the entity on the screen
    SDL_Rect dstrect;
};

#endif /* ENTITY_HPP */