#ifndef MIXER_HPP
#define MIXER_HPP

#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"

#include "appdata.hpp"

#include <string>
#include <memory>

/// @brief Audio output interface
class Mixer {
public:
    /// @brief Initializes audio
    /// @param appdata AppData shared pointer
    Mixer(const std::shared_ptr<AppData>& appdata);

    /// @brief Destructor
    ~Mixer();

    /// @brief Plays a sound
    void Play();
private:
    /// @brief AppData shared pointer
    const std::shared_ptr<AppData>& appdata;

    /// @brief SDL Audio Spec
    SDL_AudioSpec wavSpec;

    /// @brief Length of the audio file
    Uint32 wavLength;

    /// @brief Buffer of the audio file
    Uint8 *wavBuffer;
};

#endif /* MIXER_HPP */