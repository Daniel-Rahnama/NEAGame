#include "mixer.hpp"

Mixer::Mixer(const std::shared_ptr<AppData>& appdata) : appdata(appdata) {
    if (SDL_INIT_AUDIO < 0) {
        throw "Unable to initialise SDL_AUDIO";
    }
}

Mixer::~Mixer() {}

void Mixer::Play() {
    SDL_LoadWAV((appdata->Resources() + "/audio/music.wav").c_str(), &wavSpec, &wavBuffer, &wavLength);

    SDL_AudioDeviceID deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);

    int success = SDL_QueueAudio(deviceId, wavBuffer, wavLength);
    SDL_PauseAudioDevice(deviceId, 0);
}