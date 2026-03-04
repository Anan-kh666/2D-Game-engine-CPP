#include "Renderer2D.h"
#include "VertexArray.h"
#include "Shader.h"
#include <glm/gtc/matrix_transform.hpp>

struct Renderer2DData {
    VertexArray* QuadVAO;
    Shader* FlatColorShader;
    Shader* TextureShader;
};

static Renderer2DData* s_Data;

void Renderer2D::Init() {
    s_Data = new Renderer2DData();

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,
         0.5f, -0.5f, 0.0f,   1.0f, 0.0f,
         0.5f,  0.5f, 0.0f,   1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f,   0.0f, 1.0f
    };
    unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };

    s_Data->QuadVAO = new VertexArray();
    VertexBuffer* vbo = new VertexBuffer(vertices, sizeof(vertices));
    s_Data->QuadVAO->AddVertexBuffer(*vbo);
    IndexBuffer* ibo = new IndexBuffer(indices, 6);
    s_Data->QuadVAO->SetIndexBuffer(*ibo);

    std::string vertexSrc = R"(
        #version 410 core
        layout(location = 0) in vec3 a_Position;
        layout(location = 1) in vec2 a_TexCoord;
        uniform mat4 u_ViewProjection;
        uniform mat4 u_Transform; 
        out vec2 v_TexCoord;
        void main() {
            gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
            v_TexCoord = a_TexCoord;
        }
    )";

    std::string colorFragSrc = R"(
        #version 410 core
        out vec4 FragColor;
        uniform vec4 u_Color;
        void main() { FragColor = u_Color; }
    )";

    std::string texFragSrc = R"(
        #version 410 core
        out vec4 FragColor;
        in vec2 v_TexCoord;
        uniform sampler2D u_Texture;
        void main() { FragColor = texture(u_Texture, v_TexCoord); }
    )";

    s_Data->FlatColorShader = new Shader(vertexSrc, colorFragSrc);
    s_Data->TextureShader = new Shader(vertexSrc, texFragSrc);
    
    s_Data->TextureShader->Bind();
    s_Data->TextureShader->SetUniform1i("u_Texture", 0);
}

void Renderer2D::Shutdown() {
    delete s_Data->FlatColorShader;
    delete s_Data->TextureShader;
    delete s_Data->QuadVAO;
    delete s_Data;
}

void Renderer2D::BeginScene(const OrthographicCamera& camera) {
    s_Data->FlatColorShader->Bind();
    s_Data->FlatColorShader->SetUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

    s_Data->TextureShader->Bind();
    s_Data->TextureShader->SetUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
}

void Renderer2D::EndScene() {}

void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
    s_Data->FlatColorShader->Bind();
    s_Data->FlatColorShader->SetUniformVec4("u_Color", color);
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, 0.0f))
                        * glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));
    s_Data->FlatColorShader->SetUniformMat4("u_Transform", transform);

    s_Data->QuadVAO->Bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Texture2D& texture) {
    s_Data->TextureShader->Bind();

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, 0.0f))
                        * glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));
    s_Data->TextureShader->SetUniformMat4("u_Transform", transform);

    texture.Bind(0);
    s_Data->QuadVAO->Bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}