#include "Application.h"
#include "Input.h"
#include <iostream>

Application* Application::s_Instance = nullptr;
Application::Application() {
    s_Instance = this;
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW!" << std::endl;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    m_Window = glfwCreateWindow(800, 600, "2D Engine", NULL, NULL);
    if (!m_Window) {
        std::cout << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(m_Window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD!" << std::endl;
    }

    m_Running = true;
    m_LastFrameTime = 0.0f;
}

Application::~Application() {
    glfwTerminate();
}

void Application::Run() {
    int frameCount = 0;
    while (m_Running && !glfwWindowShouldClose(m_Window)) {
        float time = (float)glfwGetTime();
        float timestep = time - m_LastFrameTime;
        m_LastFrameTime = time;

        if (Input::IsKeyPressed(GLFW_KEY_ESCAPE)) {
            m_Running = false;
        }

        if (Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) {
            auto [x, y] = Input::GetMousePosition();
            std::cout << "Mouse Clicked at: (" << x << ", " << y << ")" << std::endl;
        }

        frameCount++;
        if (frameCount >= 1000) {
            std::cout << "Delta Time: " << timestep << " seconds (Approx " 
                      << (int)(1.0f / timestep) << " FPS)" << std::endl;
            frameCount = 0;
        }
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }
}