#pragma once

struct GLFWwindow;

class ImGuiLayer {
public:
    ImGuiLayer();
    ~ImGuiLayer();

    void Init(GLFWwindow* window);
    void Shutdown();
    void Begin();
    void End();
};