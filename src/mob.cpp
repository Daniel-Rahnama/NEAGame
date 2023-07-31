#include "mob.hpp"

#include <iostream>
#include <cassert>

Mob::Mob(SDL_Texture* spritesheet, uint16_t state, SDL_Rect dstrect, unsigned int layer)
    : Entity(spritesheet, {0, 512, 64, 64}, dstrect), state(state), layer(layer) {
    if (!((state ^ UP) & 0x3)) srcrect.y = 512;
    else if (!((state ^ LEFT) & 0x3)) srcrect.y = 576;
    else if (!((state ^ DOWN) & 0x3)) srcrect.y = 640;
    else if (!((state ^ RIGHT) & 0x3)) srcrect.y = 704;

    if (state & MOVING) srcrect.x = 64;
    else srcrect.x = 0;

    hitbox = { dstrect.x + 32, dstrect.y + 96, dstrect.w - 64, dstrect.h - 96 };

    health = 100;
}

void Mob::UpdateAnimation() {
    if (health <= 0) {
        if (srcrect.y == 1280) { // IF ALREADY DYING
            if (srcrect.x >= 320) { // IF DEATH ANIMATION IS DONE
                state |= DEAD;
            } else {
                srcrect.x += 64;
            }
        } else {
            if (srcrect.y >= 1344) { // IF ATTACKING
                state &= ~ATTACKING;
                
                srcrect.w = 64;
                srcrect.h = 64;
                
                dstrect.x += 128;
                dstrect.y += 128;
                dstrect.w = 128;
                dstrect.h = 128;
            }
            srcrect.y = 1280;
            srcrect.x = 0;
        }
    } else if (state & ATTACKING) {
        if (srcrect.y >= 1344) { // IF ALREADY ATTACKING
            if (srcrect.x >= 960) { // IF ATTACK ANIMATION IS DONE
                srcrect.x = (state & MOVING) ? 64 : 0;

                if (!((state ^ UP) & 0x3)) srcrect.y = 512;
                else if (!((state ^ LEFT) & 0x3)) srcrect.y = 576;
                else if (!((state ^ DOWN) & 0x3)) srcrect.y = 640;
                else if (!((state ^ RIGHT) & 0x3)) srcrect.y = 704;

                state &= ~ATTACKING;

                srcrect.w = 64;
                srcrect.h = 64;

                dstrect.x += 128;
                dstrect.y += 128;
                dstrect.w = 128;
                dstrect.h = 128;
            } else { // IF ATTACK ANIMATION IS NOT DONE
                srcrect.x += 192;
                
                if (!((state ^ UP) & 0x3)) srcrect.y = 1344;
                else if (!((state ^ LEFT) & 0x3)) srcrect.y = 1536;
                else if (!((state ^ DOWN) & 0x3)) srcrect.y = 1728;
                else if (!((state ^ RIGHT) & 0x3)) srcrect.y = 1920;
            }
        } else { // IF NOT ALREADY ATTACKING
            srcrect.x = 192;

            if (!((state ^ UP) & 0x3)) srcrect.y = 1344;
            else if (!((state ^ LEFT) & 0x3)) srcrect.y = 1536;
            else if (!((state ^ DOWN) & 0x3)) srcrect.y = 1728;
            else if (!((state ^ RIGHT) & 0x3)) srcrect.y = 1920;

            srcrect.w = 192;
            srcrect.h = 192;

            dstrect.x -= 128;
            dstrect.y -= 128;
            dstrect.w = 384;
            dstrect.h = 384;
        }
    } else if (state & MOVING) {
        if (!((state ^ UP) & 0x3)) {
            srcrect.y = 512;
            srcrect.x += 64;
            if (srcrect.x >= 576) srcrect.x = 64;
        } else if (!((state ^ LEFT) & 0x3)) {
            srcrect.y = 576;
            srcrect.x += 64;
            if (srcrect.x >= 576) srcrect.x = 64;
        } else if (!((state ^ DOWN) & 0x3)) {
            srcrect.y = 640;
            srcrect.x += 64;
            if (srcrect.x >= 576) srcrect.x = 64;
        } else if (!((state ^ RIGHT) & 0x3)) {
            srcrect.y = 704;
            srcrect.x += 64;
            if (srcrect.x >= 576) srcrect.x = 64;
        }
    } else {
        srcrect.x = 0;

        if (!((state ^ UP) & 0x3)) srcrect.y = 512;
        else if (!((state ^ LEFT) & 0x3)) srcrect.y = 576;
        else if (!((state ^ DOWN) & 0x3)) srcrect.y = 640;
        else if (!((state ^ RIGHT) & 0x3)) srcrect.y = 704;
    }
}

void Mob::Update(std::vector<std::vector<Entity*>>& entities, std::vector<Mob*>& mobs, SDL_Rect& camera) {
    if (health > 0) {
        if (state & MOVING) {
            if (!((state ^ UP) & 0x3)) {
                dstrect.y -= 4;
                hitbox.y -= 4;

                if (dstrect.y < 0) {
                    dstrect.y = 0;
                    hitbox.y = dstrect.y + (srcrect.y >= 1344) ? 288 : 96;
                }

                for (int l = layer; l < entities.size(); l++) {
                    for (Entity*& e : entities[l]) {
                        if (e == nullptr) continue;
                        if (Collision(e)) {
                            dstrect.y = e->DSTRect().y - (e->DSTRect().h - hitbox.h);
                            hitbox.y = dstrect.y + (srcrect.y >= 1344) ? 288 : 96;
                        }
                    }
                }

            } else if (!((state ^ LEFT) & 0x3)) {
                dstrect.x -= 4;
                hitbox.x -= 4;

                if (dstrect.x < 0) {
                    dstrect.x = 0;
                    hitbox.x = dstrect.x + (srcrect.y >= 1344) ? 224 : 96;
                }

                for (int l = layer; l < entities.size(); l++) {
                    for (Entity*& e : entities[l]) {
                        if (e == nullptr) continue;
                        if (Collision(e)) {
                            dstrect.x = e->DSTRect().x + ((dstrect.w - hitbox.w) / 2);
                            hitbox.x = dstrect.x + (srcrect.y >= 1344) ? 224 : 96;
                        }
                    }
                }

            } else if (!((state ^ DOWN) & 0x3)) {
                dstrect.y += 4;
                hitbox.y += 4;
                
                if (dstrect.y > camera.h - dstrect.h) {
                    dstrect.y = camera.h - dstrect.h;
                    hitbox.y = dstrect.y + (srcrect.y >= 1344) ? 288 : 96;
                }

                for (int l = layer; l < entities.size(); l++) {
                    for (Entity*& e : entities[l]) {
                        if (e == nullptr) continue;
                        if (Collision(e)) {
                            dstrect.y = e->DSTRect().y - dstrect.h;
                            hitbox.y = dstrect.y + (srcrect.y >= 1344) ? 288 : 96;
                        }
                    }
                }

            } else if (!((state ^ RIGHT) & 0x3)) {
                dstrect.x += 4;
                hitbox.x += 4;

                if (dstrect.x > camera.w - dstrect.w) {
                    dstrect.x = camera.w - dstrect.w;
                    hitbox.x = dstrect.x + (srcrect.y >= 1344) ? 224 : 96;
                }

                for (int l = layer; l < entities.size(); l++) {
                    for (Entity*& e : entities[l]) {
                        if (e == nullptr) continue;
                        if (Collision(e)) {
                            dstrect.x = e->DSTRect().x - (e->DSTRect().w + ((dstrect.w - hitbox.w) / 2));
                            hitbox.x = dstrect.x + (srcrect.y >= 1344) ? 224 : 96;
                        }
                    }
                }
            }
        }
        if (state & ATTACKING) {
            if (!((state ^ UP) & 0x3)) {
                SDL_Rect swordHitbox = { hitbox.x - 96, hitbox.y - 128, 256, 128 };
                for (Mob*& m : mobs) {
                    if (m == this) continue;
                    if (Collision(m, swordHitbox)) {
                        m->Hit(1);
                    }
                }
            } else if (!((state ^ LEFT) & 0x3)) {
                SDL_Rect swordHitbox = { hitbox.x - 160, hitbox.y - 96, 256, 128 };
                for (Mob*& m : mobs) {
                    if (m == this) continue;
                    if (Collision(m, swordHitbox)) {
                        m->Hit(1);
                    }
                }
            } else if (!((state ^ DOWN) & 0x3)) {
                SDL_Rect swordHitbox = { hitbox.x - 96, hitbox.y - 32, 256, 128 };
                for (Mob*& m : mobs) {
                    if (m == this) continue;
                    if (Collision(m, swordHitbox)) {
                        m->Hit(1);
                    }
                }
            } else if (!((state ^ RIGHT) & 0x3)) {
                SDL_Rect swordHitbox = { hitbox.x - 32, hitbox.y - 96, 256, 128 };
                for (Mob*& m : mobs) {
                    if (m == this) continue;
                    if (Collision(m, swordHitbox)) {
                        m->Hit(1);
                    }
                }
            }
        }
    } else {
        health = 0;
    }
}

const unsigned int& Mob::Layer() const {
    return layer;
}

const double& Mob::Health() const {
    return health;
}

void Mob::Hit(const double& damage) {
    health -= damage;
}

bool Mob::Collision(Entity*& e) {
    return ((hitbox.x < e->DSTRect().x + e->DSTRect().w) && (hitbox.x + hitbox.w > e->DSTRect().x)
            && (hitbox.y < e->DSTRect().y + e->DSTRect().h) && (hitbox.y + hitbox.h > e->DSTRect().y));
}

bool Mob::Collision(Mob*& m, const SDL_Rect& r) {
    return ((r.x < m->hitbox.x + m->hitbox.w) && (r.x + r.w > m->hitbox.x)
            && (r.y < m->hitbox.y + m->hitbox.h) && (r.y + r.h > m->hitbox.y));
}
