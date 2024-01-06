#include "controller.hpp"

void Controller::HandleInput(bool& running, Player*& player) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        } else if (event.type == SDL_MOUSEBUTTONDOWN) {
            switch (event.button.button) {
                case SDL_BUTTON_LEFT: player->state |= ((player->Cooldown() <= 0 && player->Stamina() >= 20) ? ATTACKING : 0); break;
                case SDL_BUTTON_RIGHT: player->state |= MOVING; break;
            }
        } else if (event.type == SDL_MOUSEBUTTONUP) {
            switch (event.button.button) {
                case SDL_BUTTON_LEFT: break;
                case SDL_BUTTON_RIGHT: player->state &= ~MOVING; break;
            }
        } else if (event.type == SDL_MOUSEMOTION) {
            if (event.motion.yrel < -15) {
                player->state = (player->state & ~(0x3));
            } else if (event.motion.xrel < -15) {
                player->state = (player->state & ~(0x2) | (0x1));
            } else if (event.motion.yrel > 15) {
                player->state = (player->state & ~(0x1) | (0x2));
            } else if (event.motion.xrel > 15) {
                player->state = (player->state | (0x3));
            }
        } else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE: running = false; break;
                case SDLK_SPACE: player->state |= ((player->Cooldown() <= 0 && player->Stamina() >= 20) ? ATTACKING : 0); break;
                case SDLK_UP:
                    player->state = (player->state & ~(0x3)) | MOVING;
                    break;
                case SDLK_LEFT:
                    player->state = (player->state & ~(0x2) | (0x1)) | MOVING;
                    break;
                case SDLK_DOWN:
                    player->state = (player->state & ~(0x1) | (0x2)) | MOVING;
                    break;
                case SDLK_RIGHT:
                    player->state = (player->state | (0x3)) | MOVING;
                    break;
            }
        } else if (event.type == SDL_KEYUP) {
            switch (event.key.keysym.sym) {
                case SDLK_UP: player->state &= ~MOVING; break;
                case SDLK_LEFT: player->state &= ~MOVING; break;
                case SDLK_DOWN: player->state &= ~MOVING; break;
                case SDLK_RIGHT: player->state &= ~MOVING; break;
            }
        }
    }
}
