#pragma once
#include "Texture2D.h"
#include <glm/glm.hpp>

struct TransformComponent {
  glm::vec3 Position = {0.0f, 0.0f, 0.0f};
  glm::vec2 Size = {1.0f, 1.0f};

  TransformComponent() = default;
  TransformComponent(const glm::vec3 &position, const glm::vec2 &size)
      : Position(position), Size(size) {}
};

struct SpriteRendererComponent {
  glm::vec4 Color = {1.0f, 1.0f, 1.0f, 1.0f};
  Texture2D *Texture = nullptr;

  SpriteRendererComponent() = default;
  SpriteRendererComponent(const glm::vec4 &color) : Color(color) {}
};

enum class Rigidbody2DType { Static = 0, Dynamic };

struct Rigidbody2DComponent {
  Rigidbody2DType Type = Rigidbody2DType::Static;
  bool FixedRotation = false;
  void *RuntimeBody = nullptr;

  Rigidbody2DComponent() = default;
};

struct BoxCollider2DComponent {
  glm::vec2 Size = {1.0f, 1.0f};
  float Density = 1.0f;
  float Friction = 0.5f;
  float Restitution = 0.0f;
  float RestitutionThreshold = 0.5f;
  void *RuntimeFixture = nullptr;

  BoxCollider2DComponent() = default;
};