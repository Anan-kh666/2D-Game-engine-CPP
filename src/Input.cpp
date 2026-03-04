#include "Input.h"
#include "Application.h"
#include <GLFW/glfw3.h>

bool Input::IsKeyPressed(int keycode) {
    auto window = Application::Get().GetWindow();
    auto state = glfwGetKey(window, keycode);
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::IsMouseButtonPressed(int button) {
    auto window = Application::Get().GetWindow();
    auto state = glfwGetMouseButton(window, button);
    return state == GLFW_PRESS;
}

std::pair<float, float> Input::GetMousePosition() {
    auto window = Application::Get().GetWindow();
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    return { (float)xpos, (float)ypos };
}