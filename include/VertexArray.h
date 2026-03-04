#pragma once
#include "Buffer.h"

class VertexArray {
public:
    VertexArray();
    ~VertexArray();

    void Bind() const;
    void Unbind() const;
    void AddVertexBuffer(const VertexBuffer& vertexBuffer);
    void SetIndexBuffer(const IndexBuffer& indexBuffer);

private:
    unsigned int m_RendererID;
};