#include "Application.h"
#include "Input.h"
#include "Renderer2D.h"
#include "OrthographicCamera.h"
#include "ImGuiLayer.h"
#include "Scene.h"
#include "Components.h"
#include "SceneSerializer.h"
#include "Texture2D.h"
#include <imgui.h>
#include <iostream>

Application* Application::s_Instance = nullptr;

OrthographicCamera* camera;
ImGuiLayer* imGuiLayer;
Scene* activeScene;
Texture2D* playerTexture;

glm::vec3 cameraPos = {0.0f, 0.0f, 0.0f};
float cameraSpeed = 5.0f;
bool isPlaying = false;

Application::Application() {
    s_Instance = this;

    if (!glfwInit()) std::cout << "Failed to initialize GLFW!" << std::endl;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    m_Window = glfwCreateWindow(800, 600, "2D Engine - Play/Stop Mode!", NULL, NULL);
    glfwMakeContextCurrent(m_Window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_Running = true;
    m_LastFrameTime = 0.0f;

    Renderer2D::Init();
    camera = new OrthographicCamera(-2.66f, 2.66f, -2.0f, 2.0f);
    imGuiLayer = new ImGuiLayer();
    imGuiLayer->Init(m_Window);

    activeScene = new Scene();
    playerTexture = new Texture2D("../assets/player.png");

    entt::entity playerEntity = activeScene->CreateEntity();
    TransformComponent& playerTransform = activeScene->GetRegistry().emplace<TransformComponent>(playerEntity);
    playerTransform.Position = { 0.0f, 5.0f, 0.0f };
    playerTransform.Size = { 1.0f, 1.0f };

    SpriteRendererComponent& playerSprite = activeScene->GetRegistry().emplace<SpriteRendererComponent>(playerEntity);
    playerSprite.Texture = playerTexture;

    Rigidbody2DComponent& playerRb = activeScene->GetRegistry().emplace<Rigidbody2DComponent>(playerEntity);
    playerRb.Type = Rigidbody2DType::Dynamic;

    BoxCollider2DComponent& playerCollider = activeScene->GetRegistry().emplace<BoxCollider2DComponent>(playerEntity);
    playerCollider.Restitution = 0.7f;

    entt::entity floorEntity = activeScene->CreateEntity();
    TransformComponent& floorTransform = activeScene->GetRegistry().emplace<TransformComponent>(floorEntity);
    floorTransform.Position = { -10.0f, -2.0f, 0.0f };
    floorTransform.Size = { 20.0f, 0.5f };

    SpriteRendererComponent& floorSprite = activeScene->GetRegistry().emplace<SpriteRendererComponent>(floorEntity);
    floorSprite.Color = { 0.2f, 0.8f, 0.2f, 1.0f };

    activeScene->GetRegistry().emplace<Rigidbody2DComponent>(floorEntity);
    activeScene->GetRegistry().emplace<BoxCollider2DComponent>(floorEntity);
}

Application::~Application() {
    delete activeScene;
    delete playerTexture;
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
        activeScene->OnUpdate(*camera, timestep);

        imGuiLayer->Begin();

        ImGui::Begin("Toolbar");

if (!isPlaying) {
    if (ImGui::Button("Play", ImVec2(80, 30))) {
        activeScene->StartPhysics();
        isPlaying = true;
    }
} else {
    if (ImGui::Button("Stop", ImVec2(80, 30))) {
        activeScene->StopPhysics();
        isPlaying = false;
    }
}

ImGui::SameLine();

if (ImGui::Button("Save Scene", ImVec2(100, 30))) {
    SceneSerializer serializer(activeScene);
    serializer.Serialize("Level_1.json");
}

ImGui::SameLine();

if (ImGui::Button("Load Scene", ImVec2(100, 30))) {
    if (isPlaying) {
        activeScene->StopPhysics();
        isPlaying = false;
    }

    delete activeScene;
    activeScene = new Scene();

    SceneSerializer serializer(activeScene);
    serializer.Deserialize("Level_1.json");

    auto view = activeScene->GetRegistry().view<Rigidbody2DComponent, SpriteRendererComponent>();
    for (auto e : view) {
        if (activeScene->GetRegistry().get<Rigidbody2DComponent>(e).Type == Rigidbody2DType::Dynamic) {
            activeScene->GetRegistry().get<SpriteRendererComponent>(e).Texture = playerTexture;
        }
    }
}

ImGui::End();

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