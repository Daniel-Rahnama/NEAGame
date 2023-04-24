#include "mob.hpp"

#include <iostream>

Mob::Mob(SDL_Texture*& spritesheet, SDL_Rect srcrect, SDL_Rect dstrect, unsigned int layer) : Entity(spritesheet, srcrect, dstrect), layer(layer) {
    hitbox = { dstrect.x + 32, dstrect.y + 96, dstrect.w - 64, dstrect.h - 96 };
}

void Mob::UpdateAnimation(const std::shared_ptr<AppData> &appdata) {
    switch (direction) {
        case Direction::NONE:
            srcrect.x = 0;
            break;
        case Direction::UP:
            srcrect.y = 512;
            srcrect.x += 64;
            if (srcrect.x >= 576) srcrect.x = 64;
            break;
        case Direction::DOWN:
            srcrect.y = 640;
            srcrect.x += 64;
            if (srcrect.x >= 576) srcrect.x = 64;
            break;
        case Direction::LEFT:
            srcrect.y = 576;
            srcrect.x += 64;
            if (srcrect.x >= 576) srcrect.x = 64;
            break;
        case Direction::RIGHT:
            srcrect.y = 704;
            srcrect.x += 64;
            if (srcrect.x >= 576) srcrect.x = 64;
            break;
    }
}

void Mob::Update(const std::shared_ptr<AppData>& appdata, std::vector<std::vector<Entity*>>& entities, SDL_Rect& camera) {
    switch(direction) {
        case Direction::UP:
            dstrect.y -= 4;
            hitbox.y -= 4;

            camera.y = dstrect.y - ((appdata->Height() - dstrect.h) / 2);

            if (dstrect.y < 0) {
                dstrect.y = 0;
                hitbox.y = dstrect.y + 96;
            }

            for (int l = layer+1; l < entities.size(); l++) {
                for (Entity*& e : entities[l]) {
                    if (e == nullptr) continue;
                    if (Collision(e)) {
                        dstrect.y = e->DSTRect().y - (e->DSTRect().h - hitbox.h);
                        hitbox.y = dstrect.y + 96;
                        camera.y = dstrect.y - ((appdata->Height() - dstrect.h) / 2);
                        break;
                    }
                }
            }
            
            if (camera.y < 0) {
                camera.y = 0;
            }

            if (camera.y > camera.h - appdata->Height()) {
                camera.y = camera.h - appdata->Height();
            }

            break;
        case Direction::DOWN:
            dstrect.y += 4;
            hitbox.y += 4;

            camera.y = dstrect.y - ((appdata->Height() - dstrect.h) / 2);
            
            if (dstrect.y > camera.h - dstrect.h) {
                dstrect.y = camera.h - dstrect.h;
                hitbox.y = dstrect.y + 96;
            }

            for (int l = layer+1; l < entities.size(); l++) {
                for (Entity*& e : entities[l]) {
                    if (e == nullptr) continue;
                    if (Collision(e)) {
                        dstrect.y = e->DSTRect().y - dstrect.h;
                        hitbox.y = dstrect.y + 96;
                        camera.y = dstrect.y - ((appdata->Height() - dstrect.h) / 2);
                        break;
                    }
                }
            }

            if (camera.y < 0) {
                camera.y = 0;
            }

            if (camera.y > camera.h - appdata->Height()) {
                camera.y = camera.h - appdata->Height();
            }

            break;
        case Direction::LEFT:
            dstrect.x -= 4;
            hitbox.x -= 4;

            camera.x = dstrect.x - ((appdata->Width() - dstrect.w) / 2);

            if (dstrect.x < 0) {
                dstrect.x = 0;
                hitbox.x = dstrect.x + 32;
            }

            for (int l = layer+1; l < entities.size(); l++) {
                for (Entity*& e : entities[l]) {
                    if (e == nullptr) continue;
                    if (Collision(e)) {
                        dstrect.x = e->DSTRect().x + ((dstrect.w - hitbox.w) / 2);
                        hitbox.x = dstrect.x + 32;
                        camera.x = dstrect.x - ((appdata->Width() - dstrect.w) / 2);
                        break;
                    }
                }
            }

            if (camera.x < 0) {
                camera.x = 0;
            }

            if (camera.x > camera.w - appdata->Width()) {
                camera.x = camera.w - appdata->Width();
            }

            break;
        case Direction::RIGHT:
            dstrect.x += 4;
            hitbox.x += 4;

            camera.x = dstrect.x - ((appdata->Width() - dstrect.w) / 2);

            if (dstrect.x > camera.w - dstrect.w) {
                dstrect.x = camera.w - dstrect.w;
                hitbox.x = dstrect.x + 32;
            }

            for (int l = layer+1; l < entities.size(); l++) {
                for (Entity*& e : entities[l]) {
                    if (e == nullptr) continue;
                    if (Collision(e)) {
                        dstrect.x = e->DSTRect().x - (e->DSTRect().w + ((dstrect.w - hitbox.w) / 2));
                        hitbox.x = dstrect.x + 32;
                        camera.x = dstrect.x - ((appdata->Width() - dstrect.w) / 2);
                        break;
                    }
                }
            }

            if (camera.x < 0) {
                camera.x = 0;
            }

            if (camera.x > camera.w - appdata->Width()) {
                camera.x = camera.w - appdata->Width();
            }

            break;
    }
}

unsigned int& Mob::Layer() {
    return layer;
}

bool Mob::Collision(Entity*& e) {
    return ((hitbox.x < e->DSTRect().x + e->DSTRect().w) && (hitbox.x + hitbox.w > e->DSTRect().x)
            && (hitbox.y < e->DSTRect().y + e->DSTRect().h) && (hitbox.y + hitbox.h > e->DSTRect().y));
}
