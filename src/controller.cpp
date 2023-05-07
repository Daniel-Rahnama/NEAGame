#include "controller.hpp"

void Controller::HandleInput(bool& running, uint16_t& state) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        } else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE: running = false; break;
                case SDLK_UP:
                    if (state & UP) break;
                    if (state & DOWN) state ^= DOWN;
                    else if (state & LEFT) state ^= LEFT;
                    else if (state & RIGHT) state ^= RIGHT;
                    state |= UP | MOVING; break;
                case SDLK_DOWN:
                    if (state & DOWN) break;
                    if (state & UP) state ^= UP;
                    else if (state & LEFT) state ^= LEFT;
                    else if (state & RIGHT) state ^= RIGHT;
                    state |= DOWN | MOVING; break;
                case SDLK_LEFT:
                    if (state & LEFT) break;
                    if (state & UP) state ^= UP;
                    else if (state & DOWN) state ^= DOWN;
                    else if (state & RIGHT) state ^= RIGHT;
                    state |= LEFT | MOVING; break;
                case SDLK_RIGHT:
                    if (state & RIGHT) break;
                    if (state & UP) state ^= UP;
                    else if (state & DOWN) state ^= DOWN;
                    else if (state & LEFT) state ^= LEFT;
                    state |= RIGHT | MOVING; break;
            }
        } else if (event.type == SDL_KEYUP) {
            switch (event.key.keysym.sym) {
                case SDLK_UP: state ^= MOVING; break;
                case SDLK_DOWN: state ^= MOVING; break;
                case SDLK_LEFT: state ^= MOVING; break;
                case SDLK_RIGHT: state ^= MOVING; break;
            }
        }
    }
}