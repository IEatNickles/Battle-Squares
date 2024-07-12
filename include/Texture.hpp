#pragma once

#include <cstdint>
#include <string>

class Texture {
    public:
        Texture(const std::string& path);

        void bind(uint32_t unit = 0) const;

    private:
        uint32_t m_TextureID;

        int m_Width, m_Height, m_Channels;
};
