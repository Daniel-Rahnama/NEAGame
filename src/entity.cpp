#include "entity.hpp"

Entity::Entity(SDL_Texture* spritesheet, SDL_Rect srcrect, SDL_Rect dstrect, const unsigned int& layer)
    : spritesheet(spritesheet), srcrect(srcrect), dstrect(dstrect), layer(layer) {

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

unsigned int& Entity::Layer() {
    return layer;
}
