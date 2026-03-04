#pragma once
#include <glm/glm.hpp>

struct TransformComponent {
    glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
    glm::vec2 Size = { 1.0f, 1.0f };

    TransformComponent() = default;
    TransformComponent(const TransformComponent&) = default;
    TransformComponent(const glm::vec3& position, const glm::vec2& size)
        : Position(position), Size(size) {}
};

struct SpriteRendererComponent {
    glm::vec4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };

    SpriteRendererComponent() = default;
    SpriteRendererComponent(const SpriteRendererComponent&) = default;
    SpriteRendererComponent(const glm::vec4& color)
        : Color(color) {}
};