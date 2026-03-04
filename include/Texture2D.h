#pragma once
#include <string>
#include <glad/glad.h>

class Texture2D {
public:
    Texture2D(const std::string& path);
    ~Texture2D();

    void Bind(unsigned int slot = 0) const;
    void Unbind() const;

    inline int GetWidth() const { return m_Width; }
    inline int GetHeight() const { return m_Height; }

private:
    unsigned int m_RendererID;
    std::string m_FilePath;
    int m_Width, m_Height, m_BPP;
};