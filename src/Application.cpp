#include "Application.h"
#include "Input.h"
#include "VertexArray.h"
#include "Buffer.h"
#include "Shader.h"
#include "Texture2D.h"
#include "OrthographicCamera.h"
#include <iostream>

Application* Application::s_Instance = nullptr;

VertexArray* quadVAO;
Shader* quadShader;
IndexBuffer* quadIBO;
Texture2D* quadTexture;
OrthographicCamera* camera; 

glm::vec3 cameraPos = {0.0f, 0.0f, 0.0f};
float cameraSpeed = 1.5f;

Application::Application() {
    s_Instance = this;

    if (!glfwInit()) std::cout << "Failed to initialize GLFW!" << std::endl;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    m_Window = glfwCreateWindow(800, 600, "2D Engine", NULL, NULL);
    glfwMakeContextCurrent(m_Window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD!" << std::endl;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_Running = true;
    m_LastFrameTime = 0.0f; 

    camera = new OrthographicCamera(-1.33f, 1.33f, -1.0f, 1.0f);

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 
         0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 
         0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 
        -0.5f,  0.5f, 0.0f,   0.0f, 1.0f  
    };

    unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };

    quadVAO = new VertexArray();
    VertexBuffer* vbo = new VertexBuffer(vertices, sizeof(vertices));
    quadVAO->AddVertexBuffer(*vbo);
    quadIBO = new IndexBuffer(indices, 6);
    quadVAO->SetIndexBuffer(*quadIBO);

    std::string vertexSrc = R"(
        #version 410 core
        layout(location = 0) in vec3 a_Position;
        layout(location = 1) in vec2 a_TexCoord;
        
        uniform mat4 u_ViewProjection; // NEW
        
        out vec2 v_TexCoord;
        
        void main() {
            // Matrix multiplication happens right to left!
            gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
            v_TexCoord = a_TexCoord;
        }
    )";

    std::string fragmentSrc = R"(
        #version 410 core
        out vec4 FragColor;
        in vec2 v_TexCoord;
        uniform sampler2D u_Texture;
        void main() {
            FragColor = texture(u_Texture, v_TexCoord); 
        }
    )";

    quadShader = new Shader(vertexSrc, fragmentSrc);
    quadTexture = new Texture2D("../assets/player.png"); 
    
    quadShader->Bind();
    quadShader->SetUniform1i("u_Texture", 0); 
}

Application::~Application() {
    glfwTerminate();
}

void Application::Run() {
    while (m_Running && !glfwWindowShouldClose(m_Window)) {
        float time = (float)glfwGetTime();
        float timestep = time - m_LastFrameTime;
        m_LastFrameTime = time;

        if (Input::IsKeyPressed(GLFW_KEY_ESCAPE)) m_Running = false;
        if (Input::IsKeyPressed(GLFW_KEY_A)) cameraPos.x -= cameraSpeed * timestep;
        if (Input::IsKeyPressed(GLFW_KEY_D)) cameraPos.x += cameraSpeed * timestep;
        if (Input::IsKeyPressed(GLFW_KEY_S)) cameraPos.y -= cameraSpeed * timestep;
        if (Input::IsKeyPressed(GLFW_KEY_W)) cameraPos.y += cameraSpeed * timestep;
        
        camera->SetPosition(cameraPos);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f); 
        glClear(GL_COLOR_BUFFER_BIT);

        quadShader->Bind();
        quadShader->SetUniformMat4("u_ViewProjection", camera->GetViewProjectionMatrix());
        
        quadTexture->Bind(0); 
        quadVAO->Bind();
        
        glDrawElements(GL_TRIANGLES, quadIBO->GetCount(), GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }
}