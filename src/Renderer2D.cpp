#include "Renderer2D.h"
#include "VertexArray.h"
#include "Shader.h"
#include <glm/gtc/matrix_transform.hpp>

static const uint32_t MaxQuads = 10000;
static const uint32_t MaxVertices = MaxQuads * 4;
static const uint32_t MaxIndices = MaxQuads * 6;

struct QuadVertex {
    glm::vec3 Position;
    glm::vec4 Color;
    glm::vec2 TexCoord;
    float TexIndex;
};

struct Renderer2DData {
    VertexArray* QuadVAO;
    VertexBuffer* QuadVBO;
    Shader* BatchShader;

    uint32_t QuadIndexCount = 0;
    QuadVertex* QuadVertexBufferBase = nullptr;
    QuadVertex* QuadVertexBufferPtr = nullptr;

    Renderer2D::Statistics Stats;
};

static Renderer2DData* s_Data;

void Renderer2D::Init() {
    s_Data = new Renderer2DData();

    s_Data->QuadVertexBufferBase = new QuadVertex[MaxVertices];

    s_Data->QuadVAO = new VertexArray();
    s_Data->QuadVBO = new VertexBuffer(MaxVertices * sizeof(QuadVertex));
    s_Data->QuadVAO->AddVertexBuffer(*s_Data->QuadVBO);

    uint32_t* quadIndices = new uint32_t[MaxIndices];
    uint32_t offset = 0;
    for (uint32_t i = 0; i < MaxIndices; i += 6) {
        quadIndices[i + 0] = offset + 0;
        quadIndices[i + 1] = offset + 1;
        quadIndices[i + 2] = offset + 2;

        quadIndices[i + 3] = offset + 2;
        quadIndices[i + 4] = offset + 3;
        quadIndices[i + 5] = offset + 0;
        offset += 4;
    }

    IndexBuffer* quadIBO = new IndexBuffer(quadIndices, MaxIndices);
    s_Data->QuadVAO->SetIndexBuffer(*quadIBO);
    delete[] quadIndices;

    std::string vertexSrc = R"(
        #version 410 core
        layout(location = 0) in vec3 a_Position;
        layout(location = 1) in vec4 a_Color;
        layout(location = 2) in vec2 a_TexCoord;
        layout(location = 3) in float a_TexIndex;

        uniform mat4 u_ViewProjection;

        out vec4 v_Color;
        out vec2 v_TexCoord;
        out float v_TexIndex;

        void main() {
            gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
            v_Color = a_Color;
            v_TexCoord = a_TexCoord;
            v_TexIndex = a_TexIndex;
        }
    )";

    std::string fragmentSrc = R"(
        #version 410 core
        out vec4 FragColor;

        in vec4 v_Color;
        in vec2 v_TexCoord;
        in float v_TexIndex;

        uniform sampler2D u_Texture;

        void main() {
            if (v_TexIndex == 0.0) {
                FragColor = v_Color;
            } else {
                FragColor = texture(u_Texture, v_TexCoord) * v_Color;
            }
        }
    )";

    s_Data->BatchShader = new Shader(vertexSrc, fragmentSrc);
    s_Data->BatchShader->Bind();
    s_Data->BatchShader->SetUniform1i("u_Texture", 0);
}

void Renderer2D::Shutdown() {
    delete[] s_Data->QuadVertexBufferBase;
    delete s_Data->BatchShader;
    delete s_Data->QuadVAO;
    delete s_Data->QuadVBO;
    delete s_Data;
}

void Renderer2D::BeginScene(const OrthographicCamera& camera) {
    s_Data->BatchShader->Bind();
    s_Data->BatchShader->SetUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

    s_Data->QuadIndexCount = 0;
    s_Data->QuadVertexBufferPtr = s_Data->QuadVertexBufferBase;
}

void Renderer2D::EndScene() {
    uint32_t dataSize = (uint8_t*)s_Data->QuadVertexBufferPtr - (uint8_t*)s_Data->QuadVertexBufferBase;

    s_Data->QuadVBO->SetData(s_Data->QuadVertexBufferBase, dataSize);

    s_Data->QuadVAO->Bind();
    glDrawElements(GL_TRIANGLES, s_Data->QuadIndexCount, GL_UNSIGNED_INT, nullptr);

    s_Data->Stats.DrawCalls++;
}

void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
    s_Data->QuadVertexBufferPtr->Position = { position.x, position.y, 0.0f };
    s_Data->QuadVertexBufferPtr->Color = color;
    s_Data->QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
    s_Data->QuadVertexBufferPtr->TexIndex = 0.0f;
    s_Data->QuadVertexBufferPtr++;

    s_Data->QuadVertexBufferPtr->Position = { position.x + size.x, position.y, 0.0f };
    s_Data->QuadVertexBufferPtr->Color = color;
    s_Data->QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
    s_Data->QuadVertexBufferPtr->TexIndex = 0.0f;
    s_Data->QuadVertexBufferPtr++;

    s_Data->QuadVertexBufferPtr->Position = { position.x + size.x, position.y + size.y, 0.0f };
    s_Data->QuadVertexBufferPtr->Color = color;
    s_Data->QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
    s_Data->QuadVertexBufferPtr->TexIndex = 0.0f;
    s_Data->QuadVertexBufferPtr++;

    s_Data->QuadVertexBufferPtr->Position = { position.x, position.y + size.y, 0.0f };
    s_Data->QuadVertexBufferPtr->Color = color;
    s_Data->QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
    s_Data->QuadVertexBufferPtr->TexIndex = 0.0f;
    s_Data->QuadVertexBufferPtr++;

    s_Data->QuadIndexCount += 6;
    s_Data->Stats.QuadCount++;
}

void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Texture2D& texture) {
    texture.Bind(0);
    constexpr glm::vec4 whiteColor = {1.0f, 1.0f, 1.0f, 1.0f};

    s_Data->QuadVertexBufferPtr->Position = { position.x, position.y, 0.0f };
    s_Data->QuadVertexBufferPtr->Color = whiteColor;
    s_Data->QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
    s_Data->QuadVertexBufferPtr->TexIndex = 1.0f;
    s_Data->QuadVertexBufferPtr++;

    s_Data->QuadVertexBufferPtr->Position = { position.x + size.x, position.y, 0.0f };
    s_Data->QuadVertexBufferPtr->Color = whiteColor;
    s_Data->QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
    s_Data->QuadVertexBufferPtr->TexIndex = 1.0f;
    s_Data->QuadVertexBufferPtr++;

    s_Data->QuadVertexBufferPtr->Position = { position.x + size.x, position.y + size.y, 0.0f };
    s_Data->QuadVertexBufferPtr->Color = whiteColor;
    s_Data->QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
    s_Data->QuadVertexBufferPtr->TexIndex = 1.0f;
    s_Data->QuadVertexBufferPtr++;

    s_Data->QuadVertexBufferPtr->Position = { position.x, position.y + size.y, 0.0f };
    s_Data->QuadVertexBufferPtr->Color = whiteColor;
    s_Data->QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
    s_Data->QuadVertexBufferPtr->TexIndex = 1.0f;
    s_Data->QuadVertexBufferPtr++;

    s_Data->QuadIndexCount += 6;
    s_Data->Stats.QuadCount++;
}

Renderer2D::Statistics Renderer2D::GetStats() { return s_Data->Stats; }
void Renderer2D::ResetStats() { s_Data->Stats.DrawCalls = 0; s_Data->Stats.QuadCount = 0; }