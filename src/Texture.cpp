#include "Texture.hpp"
#include "Shader.hpp"

#include <stb_image.h>

#include <glad/glad.h>

Texture::Texture(const std::string& path) {
    stbi_set_flip_vertically_on_load(true);
    uint8_t* pixels = stbi_load(path.c_str(), &m_Width, &m_Height, &m_Channels, 0);

    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    stbi_image_free(pixels);
}

void Texture::bind(uint32_t unit) const {
    ASSERT(unit < 32 && unit >= 0, "%d is not in the range [0-31]", unit);
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
}
