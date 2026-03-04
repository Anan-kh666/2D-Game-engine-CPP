#include "Scene.h"
#include "Components.h"
#include "Renderer2D.h"
#include <box2d/b2_world.h>
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>

Scene::Scene() {}

Scene::~Scene() {
    StopPhysics();
}

entt::entity Scene::CreateEntity() {
    return m_Registry.create();
}

void Scene::StartPhysics() {
    m_PhysicsWorld = new b2World({ 0.0f, -9.8f });

    auto view = m_Registry.view<Rigidbody2DComponent>();
    for (auto e : view) {
        TransformComponent& transform = m_Registry.get<TransformComponent>(e);
        Rigidbody2DComponent& rb2d = m_Registry.get<Rigidbody2DComponent>(e);

        b2BodyDef bodyDef;
        bodyDef.type = rb2d.Type == Rigidbody2DType::Static ? b2_staticBody : b2_dynamicBody;
        bodyDef.position.Set(transform.Position.x + transform.Size.x / 2.0f, transform.Position.y + transform.Size.y / 2.0f);
        bodyDef.angle = 0.0f;

        b2Body* body = m_PhysicsWorld->CreateBody(&bodyDef);
        body->SetFixedRotation(rb2d.FixedRotation);
        rb2d.RuntimeBody = body;

        if (m_Registry.all_of<BoxCollider2DComponent>(e)) {
            BoxCollider2DComponent& bc2d = m_Registry.get<BoxCollider2DComponent>(e);

            b2PolygonShape boxShape;
            boxShape.SetAsBox(transform.Size.x * bc2d.Size.x / 2.0f, transform.Size.y * bc2d.Size.y / 2.0f);

            b2FixtureDef fixtureDef;
            fixtureDef.shape = &boxShape;
            fixtureDef.density = bc2d.Density;
            fixtureDef.friction = bc2d.Friction;
            fixtureDef.restitution = bc2d.Restitution;
            fixtureDef.restitutionThreshold = bc2d.RestitutionThreshold;
            body->CreateFixture(&fixtureDef);
        }
    }
}

void Scene::StopPhysics() {
    if (m_PhysicsWorld) {
        delete m_PhysicsWorld;
        m_PhysicsWorld = nullptr;
    }
}

void Scene::OnUpdate(const OrthographicCamera& camera, float timestep) {
    if (m_PhysicsWorld) {
        m_PhysicsWorld->Step(timestep, 6, 2);

        auto view = m_Registry.view<Rigidbody2DComponent>();
        for (auto e : view) {
            TransformComponent& transform = m_Registry.get<TransformComponent>(e);
            Rigidbody2DComponent& rb2d = m_Registry.get<Rigidbody2DComponent>(e);
            b2Body* body = (b2Body*)rb2d.RuntimeBody;
            transform.Position.x = body->GetPosition().x - transform.Size.x / 2.0f;
            transform.Position.y = body->GetPosition().y - transform.Size.y / 2.0f;
        }
    }

    Renderer2D::BeginScene(camera);

    auto view = m_Registry.view<TransformComponent, SpriteRendererComponent>();
    for (auto entity : view) {
        auto& transform = view.get<TransformComponent>(entity);
        auto& sprite = view.get<SpriteRendererComponent>(entity);

        if (sprite.Texture != nullptr) {
            Renderer2D::DrawQuad({transform.Position.x, transform.Position.y}, transform.Size, *sprite.Texture);
        } else {
            Renderer2D::DrawQuad({transform.Position.x, transform.Position.y}, transform.Size, sprite.Color);
        }
    }

    Renderer2D::EndScene();
}