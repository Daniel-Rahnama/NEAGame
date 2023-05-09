#include "controller.hpp"

#include <iostream>

void Controller::HandleInput(bool& running, uint16_t& state) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        } else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE: running = false; break;
                case SDLK_UP:
                    state = ((((state & ~(1 << 1)) | (0 << 1)) & ~(1 << 0)) | (0 << 0)) | MOVING;
                    break;
                case SDLK_LEFT:
                    state = ((((state & ~(1 << 1)) | (0 << 1)) & ~(1 << 0)) | (1 << 0)) | MOVING;
                    break;
                case SDLK_DOWN:
                    state = ((((state & ~(1 << 1)) | (1 << 1)) & ~(1 << 0)) | (0 << 0)) | MOVING;
                    break;
                case SDLK_RIGHT:
                    state = ((((state & ~(1 << 1)) | (1 << 1)) & ~(1 << 0)) | (1 << 0)) | MOVING;
                    break;
            }
        } else if (event.type == SDL_KEYUP) {
            switch (event.key.keysym.sym) {
                case SDLK_UP: if (state & MOVING) state ^= MOVING; break;
                case SDLK_LEFT: if (state & MOVING) state ^= MOVING; break;
                case SDLK_DOWN: if (state & MOVING) state ^= MOVING; break;
                case SDLK_RIGHT: if (state & MOVING) state ^= MOVING; break;
            }
        }
    }
}
