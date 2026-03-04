#include "Application.h"
#include "Input.h"
#include "Renderer2D.h"
#include "OrthographicCamera.h"
#include "ImGuiLayer.h"
#include "Scene.h"
#include "Components.h"
#include <imgui.h>
#include <iostream>

Application* Application::s_Instance = nullptr;
OrthographicCamera* camera;
ImGuiLayer* imGuiLayer;
Scene* activeScene;
glm::vec3 cameraPos = {0.0f, 0.0f, 0.0f};
float cameraSpeed = 5.0f;

Application::Application() {
    s_Instance = this;
    if (!glfwInit()) std::cout << "Failed to initialize GLFW!" << std::endl;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    m_Window = glfwCreateWindow(800, 600, "2D Engine - ECS Powered!", NULL, NULL);
    glfwMakeContextCurrent(m_Window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    m_Running = true;
    m_LastFrameTime = 0.0f;
    Renderer2D::Init();
    camera = new OrthographicCamera(-1.33f, 1.33f, -1.0f, 1.0f);
    imGuiLayer = new ImGuiLayer();
    imGuiLayer->Init(m_Window);

    activeScene = new Scene();

    for (float y = -5.0f; y < 5.0f; y += 0.1f) {
        for (float x = -5.0f; x < 5.0f; x += 0.1f) {
            entt::entity entity = activeScene->CreateEntity();

            TransformComponent& transform = activeScene->GetRegistry().emplace<TransformComponent>(entity);
            transform.Position = { x, y, 0.0f };
            transform.Size = { 0.09f, 0.09f };

            SpriteRendererComponent& sprite = activeScene->GetRegistry().emplace<SpriteRendererComponent>(entity);
            sprite.Color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 1.0f };
        }
    }
}

Application::~Application() {
    delete activeScene;
    imGuiLayer->Shutdown();
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

        Renderer2D::ResetStats();
        activeScene->OnUpdate(*camera);

        imGuiLayer->Begin();

        ImGui::Begin("Renderer Data");
        auto stats = Renderer2D::GetStats();
        ImGui::Text("Renderer2D Stats:");
        ImGui::Text("Draw Calls: %d", stats.DrawCalls);
        ImGui::Text("Quads: %d", stats.QuadCount);
        ImGui::Separator();
        ImGui::Text("Framerate: %.1f FPS", ImGui::GetIO().Framerate);
        ImGui::End();

        imGuiLayer->End();

        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }
}