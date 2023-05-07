#include "mob.hpp"

#include <iostream>

Mob::Mob(SDL_Texture*& spritesheet, SDL_Rect srcrect, SDL_Rect dstrect, unsigned int layer) : Entity(spritesheet, srcrect, dstrect), layer(layer) {
    hitbox = { dstrect.x + 32, dstrect.y + 96, dstrect.w - 64, dstrect.h - 96 };
    state = DOWN; 
}

void Mob::UpdateAnimation(const std::shared_ptr<AppData> &appdata) {
    if (state & MOVING) {
        if (state & UP) {
            srcrect.y = 512;
            srcrect.x += 64;
            if (srcrect.x >= 576) srcrect.x = 64;
        } else if (state & DOWN) {
            srcrect.y = 640;
            srcrect.x += 64;
            if (srcrect.x >= 576) srcrect.x = 64;
        } else if (state & LEFT) {
            srcrect.y = 576;
            srcrect.x += 64;
            if (srcrect.x >= 576) srcrect.x = 64;
        } else if (state & RIGHT) {
            srcrect.y = 704;
            srcrect.x += 64;
            if (srcrect.x >= 576) srcrect.x = 64;
        }
    } else {
        srcrect.x = 0;
    }
}

void Mob::Update(const std::shared_ptr<AppData>& appdata, std::vector<std::vector<Entity*>>& entities, SDL_Rect& camera) {
    if (state & MOVING) {
        if (state & UP) {
            dstrect.y -= 4;

            if (dstrect.y < 0) {
                dstrect.y = 0;
            }

            for (int l = layer+1; l < entities.size(); l++) {
                for (Entity*& e : entities[l]) {
                    if (e == nullptr) continue;
                    if (Collision(e)) {
                        dstrect.y = e->DSTRect().y - (e->DSTRect().h - hitbox.h);
                    }
                }
            }

            hitbox.y = dstrect.y + 96;

        } else if (state & DOWN) {
            dstrect.y += 4;
            
            if (dstrect.y > camera.h - dstrect.h) {
                dstrect.y = camera.h - dstrect.h;
            }

            for (int l = layer+1; l < entities.size(); l++) {
                for (Entity*& e : entities[l]) {
                    if (e == nullptr) continue;
                    if (Collision(e)) {
                        dstrect.y = e->DSTRect().y - dstrect.h;
                    }
                }
            }

            hitbox.y = dstrect.y + 96;

        } else if (state & LEFT) {
            dstrect.x -= 4;

            if (dstrect.x < 0) {
                dstrect.x = 0;
            }

            for (int l = layer+1; l < entities.size(); l++) {
                for (Entity*& e : entities[l]) {
                    if (e == nullptr) continue;
                    if (Collision(e)) {
                        dstrect.x = e->DSTRect().x + ((dstrect.w - hitbox.w) / 2);
                    }
                }
            }

            hitbox.x = dstrect.x + 32;

        } else if (state & RIGHT) {
            dstrect.x += 4;

            if (dstrect.x > camera.w - dstrect.w) {
                dstrect.x = camera.w - dstrect.w;
            }

            for (int l = layer+1; l < entities.size(); l++) {
                for (Entity*& e : entities[l]) {
                    if (e == nullptr) continue;
                    if (Collision(e)) {
                        dstrect.x = e->DSTRect().x - (e->DSTRect().w + ((dstrect.w - hitbox.w) / 2));
                    }
                }
            }

            hitbox.x = dstrect.x + 32;

        }
    }
}

unsigned int& Mob::Layer() {
    return layer;
}

bool Mob::Collision(Entity*& e) {
    return ((hitbox.x < e->DSTRect().x + e->DSTRect().w) && (hitbox.x + hitbox.w > e->DSTRect().x)
            && (hitbox.y < e->DSTRect().y + e->DSTRect().h) && (hitbox.y + hitbox.h > e->DSTRect().y));
}
