#include "mixer.hpp"

Mixer::Mixer(std::string resources) : resources(resources) {
    if (SDL_INIT_AUDIO < 0) {
        throw "Unable to initialise SDL_AUDIO";
    }
}

Mixer::~Mixer() {}

void Mixer::Play() {
    SDL_AudioSpec wavSpec;
    Uint32 wavLength;
    Uint8 *wavBuffer;

    SDL_LoadWAV((resources + "/audio/music.wav").c_str(), &wavSpec, &wavBuffer, &wavLength);

    SDL_AudioDeviceID deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);

    int success = SDL_QueueAudio(deviceId, wavBuffer, wavLength);
    SDL_PauseAudioDevice(deviceId, 0);
}