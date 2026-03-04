#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Application {
public:
    Application();
    ~Application();

    void Run();
    inline static Application& Get() { return *s_Instance; }
    GLFWwindow* GetWindow() const { return m_Window; }

private:
    GLFWwindow* m_Window;
    bool m_Running;
    float m_LastFrameTime;
    static Application* s_Instance;
};