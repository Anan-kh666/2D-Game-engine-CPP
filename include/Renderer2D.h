#pragma once
#include "OrthographicCamera.h"
#include "Texture2D.h"
#include <glm/glm.hpp>

class Renderer2D {
public:
    static void Init();
    static void Shutdown();
    static void BeginScene(const OrthographicCamera& camera);
    static void EndScene();
    static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
    static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Texture2D& texture);

    struct Statistics {
        uint32_t DrawCalls = 0;
        uint32_t QuadCount = 0;
    };

    static Statistics GetStats();
    static void ResetStats();
};