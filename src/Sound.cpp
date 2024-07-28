#include "Sound.hpp"
#include "SDL_mixer.h"

Sound::Sound() {
    m_Sound = nullptr;
}

void Sound::dispose() {
    Mix_FreeChunk(m_Sound);
}

Sound::Sound(const std::string& path) {
    m_Sound = Mix_LoadWAV(path.c_str());
}

void Sound::play(int volume) const {
    volume = (int)((volume * 0.01f) * MIX_MAX_VOLUME);
    Mix_Volume(-1, volume);
    Mix_PlayChannel(-1, m_Sound, 0);
}
