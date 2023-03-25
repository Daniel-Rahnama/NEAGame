#ifndef MIXER_HPP
#define MIXER_HPP

#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"

#include "appdata.hpp"

#include <string>
#include <memory>

class Mixer {
public:
    Mixer(const std::shared_ptr<AppData>&);
    ~Mixer();
    void Play();
private:
    const std::shared_ptr<AppData>& appdata;

    SDL_AudioSpec wavSpec;
    Uint32 wavLength;
    Uint8 *wavBuffer;
};

#endif /* MIXER_HPP */