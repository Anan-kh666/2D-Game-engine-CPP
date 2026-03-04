#pragma once
#include <entt/entt.hpp>
#include "OrthographicCamera.h"

class b2World;

class Scene {
public:
    Scene();
    ~Scene();

    entt::entity CreateEntity();

    void StartPhysics();
    void StopPhysics();

    void OnUpdate(const OrthographicCamera& camera, float timestep);
    entt::registry& GetRegistry() { return m_Registry; }

private:
    entt::registry m_Registry;
    b2World* m_PhysicsWorld = nullptr;
};