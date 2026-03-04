#pragma once

// --- Vertex Buffer ---
class VertexBuffer {
public:
    VertexBuffer(const float* vertices, unsigned int size);
    VertexBuffer(unsigned int size);
    ~VertexBuffer();

    void Bind() const;
    void Unbind() const;

    void SetData(const void* data, unsigned int size);

private:
    unsigned int m_RendererID;
};

// --- Index Buffer ---
class IndexBuffer {
public:
    IndexBuffer(const unsigned int* indices, unsigned int count);
    ~IndexBuffer();

    void Bind() const;
    void Unbind() const;

    inline unsigned int GetCount() const { return m_Count; }

private:
    unsigned int m_RendererID;
    unsigned int m_Count;
};