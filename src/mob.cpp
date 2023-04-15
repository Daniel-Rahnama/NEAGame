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

void Mob::Update(const std::shared_ptr<AppData>& appdata) {
    switch(direction) {
        case Direction::UP: MoveUp(appdata->Height()); break;
        case Direction::DOWN: MoveDown(appdata->Height()); break;
        case Direction::LEFT: MoveLeft(appdata->Width()); break;
        case Direction::RIGHT: MoveRight(appdata->Width()); break;
    }
}

void Mob::MoveUp(const unsigned int& bound) {
    dstrect.y -= 4;
    if (dstrect.y < 0) dstrect.y = 0;
}

void Mob::MoveDown(const unsigned int& bound) {
    dstrect.y += 4;
    if (dstrect.y > bound - dstrect.h) dstrect.y = bound - dstrect.h;
}

void Mob::MoveLeft(const unsigned int& bound) {
    dstrect.x -= 4;
    if (dstrect.x < 0) dstrect.x = 0;
}

void Mob::MoveRight(const unsigned int& bound) {
    dstrect.x += 4;
    if (dstrect.x > bound - dstrect.w) dstrect.x = bound - dstrect.w;
}
