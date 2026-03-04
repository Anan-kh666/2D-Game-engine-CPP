#include "Application.h"
#include "Input.h"
#include "Renderer2D.h"
#include "Texture2D.h"
#include "OrthographicCamera.h"
#include <iostream>

Application* Application::s_Instance = nullptr;

OrthographicCamera* camera;
Texture2D* playerTexture;
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
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_Running = true;
    m_LastFrameTime = 0.0f; 

    Renderer2D::Init();

    camera = new OrthographicCamera(-1.33f, 1.33f, -1.0f, 1.0f);
    playerTexture = new Texture2D("../assets/player.png"); 
}

Application::~Application() {
    Renderer2D::Shutdown();
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

        Renderer2D::BeginScene(*camera);

        Renderer2D::DrawQuad({0.0f, 0.0f}, {2.0f, 2.0f}, {0.2f, 0.3f, 0.8f, 1.0f});

        Renderer2D::DrawQuad({0.0f, 0.0f}, {0.5f, 0.5f}, *playerTexture);

        Renderer2D::EndScene();
        // -----------------------------------

        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }
}