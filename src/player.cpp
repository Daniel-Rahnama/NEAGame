#include "player.hpp"
#include "mob.hpp"

#include <iostream>

void Player::Update(AppData& appdata, std::vector<std::vector<Entity*>>& entities, std::vector<Mob*>& mobs, SDL_Rect& camera) {
    if (health > 0) {
        if (state & MOVING) {
            if (!((state ^ UP) & 0x3)) {
                dstrect.y -= 4;
                hitbox.y -= 4;

                if (dstrect.y < ((srcrect.y >= 1344) ? -128 : 0)) {
                    dstrect.y = ((srcrect.y >= 1344) ? -128 : 0);
                    hitbox.y = dstrect.y + ((srcrect.y >= 1344) ? 224 : 96);
                }

                for (int l = layer; l < entities.size(); l++) {
                    for (Entity*& e : entities[l]) {
                        if (e == nullptr) continue;
                        if (Collision(e)) {
                            dstrect.y = (e->DSTRect().y - (e->DSTRect().h - hitbox.h)) - ((srcrect.y >= 1344) ? 128 : 0);
                            hitbox.y = dstrect.y + ((srcrect.y >= 1344) ? 224 : 96);
                            break;
                        }
                    }
                }

                for (Mob*& m : mobs) {
                    if (m == nullptr) continue;
                    if (Collision(m->hitbox)) {
                        dstrect.y = (m->hitbox.y + m->hitbox.h + hitbox.h - dstrect.h) + ((srcrect.y >= 1344) ? 128 : 0);
                        hitbox.y = dstrect.y + ((srcrect.y >= 1344) ? 224 : 96);
                        break;
                    }
                }

                camera.y = dstrect.y - ((appdata.Height() - dstrect.h) / 2);

                if (camera.y < 0) {
                    camera.y = 0;
                }

                if (camera.y > camera.h - appdata.Height()) {
                    camera.y = camera.h - appdata.Height();
                }

            } else if (!((state ^ LEFT) & 0x3)) {
                dstrect.x -= 4;
                hitbox.x -= 4;

                if (dstrect.x < ((srcrect.y >= 1344) ? -128 : 0)) {
                    dstrect.x = ((srcrect.y >= 1344) ? -128 : 0);
                    hitbox.x = dstrect.x + ((srcrect.y >= 1344) ? 160 : 32);
                }

                for (int l = layer; l < entities.size(); l++) {
                    for (Entity*& e : entities[l]) {
                        if (e == nullptr) continue;
                        if (Collision(e)) {
                            dstrect.x = (e->DSTRect().x + 32) - ((srcrect.y >= 1344) ? 128 : 0);
                            hitbox.x = dstrect.x + ((srcrect.y >= 1344) ? 160 : 32);
                            break;
                        }
                    }
                }

                for (Mob*& m : mobs) {
                    if (m == nullptr) continue;
                    if (Collision(m->hitbox)) {
                        dstrect.x = (m->hitbox.x + 32) - ((srcrect.y >= 1344) ? 128 : 0);
                        hitbox.x = dstrect.x + ((srcrect.y >= 1344) ? 160 : 32);
                        break;
                    }
                }

                camera.x = dstrect.x - ((appdata.Width() - dstrect.w) / 2);

                if (camera.x < 0) {
                    camera.x = 0;
                }

                if (camera.x > camera.w - appdata.Width()) {
                    camera.x = camera.w - appdata.Width();
                }

            } else if (!((state ^ DOWN) & 0x3)) {
                dstrect.y += 4;
                hitbox.y += 4;

                if (dstrect.y > camera.h - dstrect.h + ((srcrect.y >= 1344) ? 128 : 0)) {
                    dstrect.y = camera.h - dstrect.h + ((srcrect.y >= 1344) ? 128 : 0);
                    hitbox.y = dstrect.y + ((srcrect.y >= 1344) ? 224 : 96);
                }

                for (int l = layer; l < entities.size(); l++) {
                    for (Entity*& e : entities[l]) {
                        if (e == nullptr) continue;
                        if (Collision(e)) {
                            dstrect.y = (e->DSTRect().y - dstrect.h) + ((srcrect.y >= 1344) ? 128 : 0);
                            hitbox.y = dstrect.y + ((srcrect.y >= 1344) ? 224 : 96);
                            break;
                        }
                    }
                }
                
                for (Mob*& m : mobs) {
                    if (m == nullptr) continue;
                    if (Collision(m->hitbox)) {
                        dstrect.y = (m->hitbox.y - dstrect.h) + ((srcrect.y >= 1344) ? 128 : 0);
                        hitbox.y = dstrect.y + ((srcrect.y >= 1344) ? 224 : 96);
                        break;
                    }
                }

                camera.y = dstrect.y - ((appdata.Height() - dstrect.h) / 2);

                if (camera.y < 0) {
                    camera.y = 0;
                }

                if (camera.y > camera.h - appdata.Height()) {
                    camera.y = camera.h - appdata.Height();
                }

            } else if (!((state ^ RIGHT) & 0x3)) {
                dstrect.x += 4;
                hitbox.x += 4;

                if (dstrect.x > camera.w - dstrect.w + ((srcrect.y >= 1344) ? 128 : 0)) {
                    dstrect.x = camera.w - dstrect.w + ((srcrect.y >= 1344) ? 128 : 0);
                    hitbox.x = dstrect.x + ((srcrect.y >= 1344) ? 160 : 32);
                }

                for (int l = layer; l < entities.size(); l++) {
                    for (Entity*& e : entities[l]) {
                        if (e == nullptr) continue;
                        if (Collision(e)) {
                            dstrect.x = (e->DSTRect().x - (e->DSTRect().w + 32)) - ((srcrect.y >= 1344) ? 128 : 0);
                            hitbox.x = dstrect.x + ((srcrect.y >= 1344) ? 160 : 32);
                            break;
                        }
                    }
                }

                for (Mob*& m : mobs) {
                    if (m == nullptr) continue;
                    if (Collision(m->hitbox)) {
                        dstrect.x = (m->hitbox.x - (m->hitbox.w + 32)) - ((srcrect.y >= 1344) ? 128 : 0);
                        hitbox.x = dstrect.x + ((srcrect.y >= 1344) ? 160 : 32);
                        break;
                    }
                }                

                camera.x = dstrect.x - ((appdata.Width() - dstrect.w) / 2);

                if (camera.x < 0) {
                    camera.x = 0;
                }

                if (camera.x > camera.w - appdata.Width()) {
                    camera.x = camera.w - appdata.Width();
                }
            }
        }
        if (state & ATTACKING) {
            if (!((state ^ UP) & 0x3)) {
                SDL_Rect swordHitbox = { hitbox.x - 96, hitbox.y - 128, 256, 128 };
                for (Mob*& m : mobs) {
                    if (Collision(m, swordHitbox)) {
                        m->Hit(1);
                    }
                }
            } else if (!((state ^ LEFT) & 0x3)) {
                SDL_Rect swordHitbox = { hitbox.x - 160, hitbox.y - 96, 256, 128 };
                for (Mob*& m : mobs) {
                    if (Collision(m, swordHitbox)) {
                        m->Hit(1);
                    }
                }
            } else if (!((state ^ DOWN) & 0x3)) {
                SDL_Rect swordHitbox = { hitbox.x - 96, hitbox.y - 32, 256, 128 };
                for (Mob*& m : mobs) {
                    if (Collision(m, swordHitbox)) {
                        m->Hit(1);
                    }
                }
            } else if (!((state ^ RIGHT) & 0x3)) {
                SDL_Rect swordHitbox = { hitbox.x - 32, hitbox.y - 96, 256, 128 };
                for (Mob*& m : mobs) {
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

bool Player::EvaluateCollision(const SDL_Rect &rect, const int &damage) {
    if (Collision(rect)) {
        if (damage) Hit(damage);
        return true;
    } else {
        return false;
    }
}

bool Player::Collision(const SDL_Rect& rect) {
    return ((hitbox.x < rect.x + rect.w) && (hitbox.x + hitbox.w > rect.x)
        && (hitbox.y < rect.y + rect.h) && (hitbox.y + hitbox.h > rect.y));
}
