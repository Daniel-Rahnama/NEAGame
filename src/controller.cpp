#include "controller.hpp"

void Controller::HandleInput(bool& running, Direction& direction) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        } else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE: running = false; break;
                case SDLK_UP: direction = Direction::UP; break;
                case SDLK_DOWN: direction = Direction::DOWN; break;
                case SDLK_LEFT: direction = Direction::LEFT; break;
                case SDLK_RIGHT: direction = Direction::RIGHT; break;
            }
        } else if (event.type == SDL_KEYUP) {
            switch (event.key.keysym.sym) {
                case SDLK_UP: direction = Direction::NONE; break;
                case SDLK_DOWN: direction = Direction::NONE; break;
                case SDLK_LEFT: direction = Direction::NONE; break;
                case SDLK_RIGHT: direction = Direction::NONE; break;
            }
        }
    }
}