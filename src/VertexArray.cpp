#include "VertexArray.h"
#include <glad/glad.h>

VertexArray::VertexArray() {
    glGenVertexArrays(1, &m_RendererID);
}

VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &m_RendererID);
}

void VertexArray::Bind() const {
    glBindVertexArray(m_RendererID);
}

void VertexArray::Unbind() const {
    glBindVertexArray(0);
}

void VertexArray::AddVertexBuffer(const VertexBuffer& vertexBuffer) {
    Bind();
    vertexBuffer.Bind();
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void*)(3 * sizeof(float)));
}

void VertexArray::SetIndexBuffer(const IndexBuffer& indexBuffer) {
    Bind();
    indexBuffer.Bind();
}