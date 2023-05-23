#include "controller.hpp"

#include <iostream>

void Controller::HandleInput(bool& running, uint16_t& state) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        } else if (event.type == SDL_MOUSEBUTTONDOWN) {
            switch (event.button.button) {
                case SDL_BUTTON_LEFT: state |= ATTACKING; break;
                case SDL_BUTTON_RIGHT: state |= MOVING; break;
            }
        } else if (event.type == SDL_MOUSEBUTTONUP) {
            switch (event.button.button) {
                case SDL_BUTTON_LEFT: break;
                case SDL_BUTTON_RIGHT: state &= ~MOVING; break;
            }
        } else if (event.type == SDL_MOUSEMOTION) {
            if (event.motion.yrel < -15) {
                state = (state & ~(0x3));
            } else if (event.motion.xrel < -15) {
                state = (state & ~(0x2) | (0x1));
            } else if (event.motion.yrel > 15) {
                state = (state & ~(0x1) | (0x2));
            } else if (event.motion.xrel > 15) {
                state = (state | (0x3));
            }
        } else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE: running = false; break;
                case SDLK_SPACE: state |= ATTACKING; break;
                case SDLK_UP:
                    state = (state & ~(0x3)) | MOVING;
                    break;
                case SDLK_LEFT:
                    state = (state & ~(0x2) | (0x1)) | MOVING;
                    break;
                case SDLK_DOWN:
                    state = (state & ~(0x1) | (0x2)) | MOVING;
                    break;
                case SDLK_RIGHT:
                    state = (state | (0x3)) | MOVING;
                    break;
            }
        } else if (event.type == SDL_KEYUP) {
            switch (event.key.keysym.sym) {
                case SDLK_UP: state &= ~MOVING; break;
                case SDLK_LEFT: state &= ~MOVING; break;
                case SDLK_DOWN: state &= ~MOVING; break;
                case SDLK_RIGHT: state &= ~MOVING; break;
            }
        }
    }
}
