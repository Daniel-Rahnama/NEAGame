#include "player.hpp"
#include "mob.hpp"

#include <iostream>

void Player::Update(const std::unique_ptr<AppData>& appdata, std::vector<std::vector<Entity*>>& entities, SDL_Rect& camera) {
    if (state & MOVING) {
        if (!((state ^ DOWN) & 0x3)) {
            dstrect.y += 4;
            hitbox.y += 4;

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
                        break;
                    }
                }
            }

            camera.y = dstrect.y - ((appdata->Height() - dstrect.h) / 2);

            if (camera.y < 0) {
                camera.y = 0;
            }

            if (camera.y > camera.h - appdata->Height()) {
                camera.y = camera.h - appdata->Height();
            }

        } else if (!((state ^ UP) & 0x3)) {
            dstrect.y -= 4;
            hitbox.y -= 4;

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
                        break;
                    }
                }
            }

            camera.y = dstrect.y - ((appdata->Height() - dstrect.h) / 2);

            if (camera.y < 0) {
                camera.y = 0;
            }

            if (camera.y > camera.h - appdata->Height()) {
                camera.y = camera.h - appdata->Height();
            }

        } else if (!((state ^ LEFT) & 0x3)) {
            dstrect.x -= 4;
            hitbox.x -= 4;

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
                        break;
                    }
                }
            }

            camera.x = dstrect.x - ((appdata->Width() - dstrect.w) / 2);

            if (camera.x < 0) {
                camera.x = 0;
            }

            if (camera.x > camera.w - appdata->Width()) {
                camera.x = camera.w - appdata->Width();
            }

        } else if (!((state ^ RIGHT) & 0x3)) {
            dstrect.x += 4;
            hitbox.x += 4;

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
                        break;
                    }
                }
            }

            camera.x = dstrect.x - ((appdata->Width() - dstrect.w) / 2);

            if (camera.x < 0) {
                camera.x = 0;
            }

            if (camera.x > camera.w - appdata->Width()) {
                camera.x = camera.w - appdata->Width();
            }
        }
    }
}