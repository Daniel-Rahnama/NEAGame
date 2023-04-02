#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <string>
#include "SDL2/SDL.h"

class Entity {
public:
    Entity(SDL_Texture*&, SDL_Rect, SDL_Rect);
    ~Entity();

    SDL_Texture*& Spritesheet();
    SDL_Rect& SRCRect();
    SDL_Rect& DSTRect();
protected:
    SDL_Texture*& spritesheet;
    SDL_Rect srcrect; 
    SDL_Rect dstrect;
};

#endif /* ENTITY_HPP */