#include "entity.hpp"

Entity::Entity(SDL_Texture* spritesheet, SDL_Rect srcrect, SDL_Rect dstrect) : spritesheet(spritesheet), srcrect(srcrect), dstrect(dstrect) {

}

Entity::~Entity() {

}

SDL_Texture*& Entity::Spritesheet() {
    return spritesheet;
}

SDL_Rect& Entity::SRCRect() {
    return srcrect;
}

SDL_Rect& Entity::DSTRect() {
    return dstrect;
}
