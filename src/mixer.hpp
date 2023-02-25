#ifndef MIXER_HPP
#define MIXER_HPP

#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"

#include <string>

class Mixer {
public:
    Mixer(const char*);
    ~Mixer();
    void Play();
private:
    std::string resources;
};

#endif /* MIXER_HPP */