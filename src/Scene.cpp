#include "Scene.h"
#include "Components.h"
#include "Renderer2D.h"

Scene::Scene() {}
Scene::~Scene() {}

entt::entity Scene::CreateEntity() {
    return m_Registry.create();
}

void Scene::OnUpdate(const OrthographicCamera& camera) {
    Renderer2D::BeginScene(camera);

    auto view = m_Registry.view<TransformComponent, SpriteRendererComponent>();

    for (auto entity : view) {
        auto& transform = view.get<TransformComponent>(entity);
        auto& sprite = view.get<SpriteRendererComponent>(entity);

        Renderer2D::DrawQuad(
            {transform.Position.x, transform.Position.y},
            transform.Size,
            sprite.Color
        );
    }

    Renderer2D::EndScene();
}