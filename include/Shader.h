#pragma once
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>

class Shader {
public:
    Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
    ~Shader();

    void Bind() const;
    void Unbind() const;
    void SetUniform1i(const std::string& name, int value);
    void SetUniformMat4(const std::string& name, const glm::mat4& matrix);
    void SetUniformVec4(const std::string& name, const glm::vec4& value);

private:
    unsigned int m_RendererID;
};