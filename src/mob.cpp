#include "mob.hpp"

void Mob::UpdateAnimation(const std::shared_ptr<AppData>& appdata) {
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

void Mob::Update(const std::shared_ptr<AppData>& appdata, std::vector<std::vector<Entity*>>& entities) {
    switch(direction) {
        case Direction::UP:
            dstrect.y -= 4;

            if (dstrect.y < 0) dstrect.y = 0;

            for (int l = layer+1; l < entities.size(); l++) {
                for (Entity*& e : entities[l]) {
                    if (e == nullptr) continue;
                    if (Collision(e)) {
                        dstrect.y = e->DSTRect().y + e->DSTRect().h;
                        break;
                    }
                }
            }

            break;
        case Direction::DOWN:
            dstrect.y += 4;

            if (dstrect.y > appdata->Height() - dstrect.h) dstrect.y = appdata->Height() - dstrect.h;

            for (int l = layer+1; l < entities.size(); l++) {
                for (Entity*& e : entities[l]) {
                    if (e == nullptr) continue;
                    if (Collision(e)) {
                        dstrect.y = e->DSTRect().y - dstrect.h;
                        break;
                    }
                }
            }

            break;
        case Direction::LEFT:
            dstrect.x -= 4;
            if (dstrect.x < 0) dstrect.x = 0;

            for (int l = layer+1; l < entities.size(); l++) {
                for (Entity*& e : entities[l]) {
                    if (e == nullptr) continue;
                    if (Collision(e)) {
                        dstrect.x = e->DSTRect().x + e->DSTRect().w;
                        break;
                    }
                }
            }

            break;
        case Direction::RIGHT:
            dstrect.x += 4;

            if (dstrect.x > appdata->Width() - dstrect.w) dstrect.x = appdata->Width() - dstrect.w;

            for (int l = layer+1; l < entities.size(); l++) {
                for (Entity*& e : entities[l]) {
                    if (e == nullptr) continue;
                    if (Collision(e)) {
                        dstrect.x = e->DSTRect().x - dstrect.w;
                        break;
                    }
                }
            }

            break;
    }
}

bool Mob::Collision(Entity*& e) {
    return ((dstrect.x < e->DSTRect().x + e->DSTRect().w) && (dstrect.x + dstrect.w > e->DSTRect().x)
                        && (dstrect.y < e->DSTRect().y + e->DSTRect().h) && (dstrect.y + dstrect.h > e->DSTRect().y));
}
