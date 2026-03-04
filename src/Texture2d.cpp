#include "Texture2D.h"
#include "stb_image.h"
#include <iostream>

Texture2D::Texture2D(const std::string& path)
    : m_RendererID(0), m_FilePath(path), m_Width(0), m_Height(0), m_BPP(0)
{
    stbi_set_flip_vertically_on_load(1);

    unsigned char* localBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);

    if (!localBuffer) {
        std::cout << "Failed to load texture: " << path << std::endl;
        return;
    }

    glGenTextures(1, &m_RendererID);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer);
    glBindTexture(GL_TEXTURE_2D, 0);


    if (localBuffer) {
        stbi_image_free(localBuffer);
    }
}

Texture2D::~Texture2D() {
    glDeleteTextures(1, &m_RendererID);
}

void Texture2D::Bind(unsigned int slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

void Texture2D::Unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}