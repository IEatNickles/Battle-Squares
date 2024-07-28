#pragma once

#include <string>

struct Mix_Chunk;

class Sound {
    public:
        Sound();
        Sound(const std::string& path);

        void dispose();
        void play(int volume = 100) const;

    private:
        Mix_Chunk *m_Sound;
};

